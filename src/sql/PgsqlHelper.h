#pragma once
#include "Logger.h"
#include "NonCopyable.h"
#include "PgsqlConnectionPool.h"
#include "json/json.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <iostream>
#include <list>
class PgsqlHelper : public AppFrame::NonCopyable
{
  public:
    static PgsqlHelper &getSqlHelper()
    {
        static PgsqlHelper helper;
        return helper;
    }

    ~PgsqlHelper()
    {
        delete pool_;
    }
    bool initSqlHelper(const std::string &host, uint16_t port, const std::string &dbName, const std::string &user,
                       const std::string &password)
    {
        pool_ = new PgsqlConnectionPool(host.c_str(), port, dbName.c_str(), user.c_str(), password.c_str(), 20, 5000);
        return pool_->getCount();
    }
    bool createTable(const std::string &tableName, std::list<std::string> &&fields) // ok
    {
        bool ret = true;
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
        {
            return false;
        }
        QSqlQuery query(*connect);

        // 构建创建表的SQL语句
        QString sql = fmt::format("CREATE TABLE IF NOT EXISTS \"{}\" (", tableName.c_str()).c_str();
        for (const auto &field : fields)
        {
            sql += field.c_str() + QString(", ");
        }
        sql.chop(2); // 去除最后的逗号和空格
        sql += ")";
        sql += ";";
        if (!query.exec(sql))
        {
            LogError("Failed to create table: {}", query.lastError().text().toStdString());
            ret = false;
        }
        pool_->releaseConnection(connect);

        return ret;
    }

    bool insertData(const std::string &tableName, const QVariantMap &data)
    {
        bool ret = true;
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
        {
            return false;
        }

        QSqlQuery query(*connect);

        QString sqlQuery = "INSERT INTO " + QString::fromStdString(tableName) + " (";
        QString placeholders;
        QStringList keys;

        for (const QString &key : data.keys())
        {
            if (data[key].isNull() || !data[key].isValid() || data[key] == "")
            {
                continue; // 跳过空值字段
            }

            keys << key;
            placeholders += "?,";
        }

        if (keys.isEmpty())
        {
            // 所有字段都为空值，不执行插入操作
            pool_->releaseConnection(connect);
            return true;
        }

        sqlQuery += keys.join(", ") + ") VALUES (" + placeholders.left(placeholders.length() - 1) + ");";
        query.prepare(sqlQuery);
        for (const QString &key : keys)
        {
            query.addBindValue(data[key]);
        }

        if (!query.exec())
        {
            LogError("Failed to insert data: {}", query.lastError().text().toStdString());
            ret = false;
        }
        pool_->releaseConnection(connect);
        return ret;
    }

    bool insertMultipleData(const std::string &tableName, const QList<QVariantMap> &dataList)
    {
        bool ret = true;
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
        {
            return false;
        }

        QSqlQuery query(*connect);
        QString sqlQuery = "INSERT INTO " + QString::fromStdString(tableName) + " (";
        QString placeholders;
        QStringList keys;

        if (dataList.isEmpty())
        {
            // No data to insert, return true as a special case
            pool_->releaseConnection(connect);
            return true;
        }

        // Use the first QVariantMap in dataList to extract keys
        const QVariantMap &firstData = dataList.first();
        for (const QString &key : firstData.keys())
        {
            keys << key;
            placeholders += "?,";
        }

        sqlQuery += keys.join(", ") + ") VALUES ";

        // Add placeholders for each row
        for (int i = 0; i < dataList.size(); ++i)
        {
            placeholders += "(" + placeholders.left(placeholders.length() - 1) + "),";
        }

        sqlQuery += placeholders.left(placeholders.length() - 1); // Remove the trailing comma

        query.prepare(sqlQuery);

        // Bind values for each row
        int index = 0;
        for (const QVariantMap &data : dataList)
        {
            for (const QString &key : keys)
            {
                query.bindValue(index++, data[key]);
            }
        }

        if (!query.execBatch())
        {
            LogError("Failed to insert data: {}", query.lastError().text().toStdString());
            ret = false;
        }

        pool_->releaseConnection(connect);
        return ret;
    }

    bool updateData(const std::string &tableName, const QVariantMap &data, const std::string &&condition)
    {
        bool ret = true;
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
        {
            return false;
        }

        QSqlQuery query(*connect);
        QStringList keys;

        // 构建更新数据的SQL语句
        QString sqlQuery = "UPDATE " + QString::fromStdString(tableName) + " SET ";
        for (const QString &key : data.keys())
        {
            if (data[key].isNull() || !data[key].isValid() || data[key] == "")
            {
                continue; // 跳过空值字段
            }
            keys << key;
        }

        if (keys.isEmpty())
        {
            // 所有字段都为空值，不执行插入操作
            pool_->releaseConnection(connect);
            return true;
        }
        sqlQuery += keys.join(" = ?, ");
        sqlQuery.chop(2); // 去除最后的逗号和空格
        sqlQuery += QString(" WHERE ") + condition.c_str();
        query.prepare(sqlQuery);
        for (const QString &key : keys)
        {
            query.addBindValue(data[key]);
        }

        if (!query.exec())
        {
            LogError("Failed to update data: {}", query.lastError().text().toStdString());
            ret = false;
        }
        pool_->releaseConnection(connect);
        return ret;
    }

    bool upsertData(const std::string &tableName, const Json::Value &&data) // ok
    {
        bool ret = true;
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
            return false;

        QSqlQuery query(*connect);
        QVariantList params;

        // 构建UPSERT语句
        QString upsertSql = fmt::format("INSERT INTO \"{}\" (", tableName.c_str()).c_str();
        QString updateSql = ") VALUES (";
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            upsertSql += it.name() + ", ";
            updateSql += "?, ";
            QVariant curVal;
            if (it->isBool())
                curVal = it->asBool();
            if (it->isInt())
                curVal = it->asInt();
            if (it->isDouble())
                curVal = it->asFloat();
            if (it->isString())
                curVal = it->asCString();
            params << curVal;
        }
        upsertSql.chop(2); // 去除最后的逗号和空格
        updateSql.chop(2); // 去除最后的逗号和空格
        upsertSql += updateSql + ");";
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            upsertSql += it.name() + " = VALUES(" + it.name() + "), ";
        }
        upsertSql.chop(2); // 去除最后的逗号和空格
        query.prepare(upsertSql);
        // qDebug() << "sql upsert: " << upsertSql;

        for (int i = 0; i < params.size(); ++i)
        {
            query.addBindValue(params[i]);
        }

        if (!query.exec())
        {
            LogError("Failed to upsert data: {}", query.lastError().text().toStdString());
            ret = false;
        }
        std::cout << upsertSql.toStdString() << std::endl;
        pool_->releaseConnection(connect);
        return ret;
    }

    bool deleteData(const std::string &tableName, const std::string &condition) // ok
    {
        bool ret = true;
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
            return false;

        QSqlQuery query(*connect);

        // 构建删除数据的SQL语句
        QString sql = "DELETE FROM " + QString::fromStdString(tableName) + " WHERE " + condition.c_str();

        query.prepare(sql);
        qDebug() << "delete user sql" << sql;
        if (!query.exec())
        {
            LogError("Failed to delete data: {}", query.lastError().text().toStdString());
            ret = false;
        }

        pool_->releaseConnection(connect);
        std::cout << sql.toStdString() << std::endl;
        return ret;
    }

    Json::Value selectData(const std::string &tableName, const std::string &&condition, // ok
                           const std::string &&orderBy = "")
    {
        Json::Value jsVal;
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
        {
            LogError("Sql poll init failed!");
            return jsVal;
        }
        QSqlQuery query(*connect);

        // 构建查询数据的SQL语句
        QString sql = QString("SELECT * FROM ") + tableName.c_str();
        if (!condition.empty())
        {
            sql += " WHERE " + condition;
        }
        if (!orderBy.empty())
        {
            sql += " ORDER BY " + orderBy;
        }
        // qDebug() << "sql statement: " << sql;
        query.prepare(sql);
        if (query.exec())
        {
            while (query.next())
            {
                QSqlRecord record = query.record();
                Json::Value jsItem;
                for (int i = 0; i < record.count(); ++i)
                {
                    QString fieldName = record.fieldName(i);
                    jsItem[fieldName.toStdString()] = std::move(query.value(i).toString().toStdString());
                }
                jsVal.append(jsItem);
                jsItem.clear();
            }
        }
        else
        {
            LogError("Failed to select data: {}", query.lastError().text().toStdString());
        }

        pool_->releaseConnection(connect);
        return jsVal;
    }

    Json::Value selectDataPaged(const std::string &tableName, const int pageSize, const int pageNumber,
                                const std::string &&condition = "", const std::string &&orderBy = "") // ok
    {
        Json::Value jsVal;

        // 计算要查询的起始行和偏移量
        int offset = (pageNumber - 1) * pageSize;

        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
        {
            LogError("Sql pool init failed!");
            return jsVal;
        }
        QSqlQuery query(*connect);

        // 构建查询数据的SQL语句，包括分页信息
        QString sql = QString("SELECT * FROM %1").arg(tableName.c_str());
        if (!condition.empty())
        {
            sql += " WHERE " + condition;
        }
        if (!orderBy.empty())
        {
            sql += " ORDER BY " + orderBy;
        }
        // 添加分页限制
        sql += QString(" LIMIT %1 OFFSET %2").arg(pageSize).arg(offset);

        query.prepare(sql);
        if (query.exec())
        {
            while (query.next())
            {
                QSqlRecord record = query.record();
                Json::Value jsItem;
                for (int i = 0; i < record.count(); ++i)
                {
                    QString fieldName = record.fieldName(i);
                    jsItem[fieldName.toStdString()] = std::move(query.value(i).toString().toStdString());
                }
                jsVal.append(jsItem);
            }
        }
        else
        {
            LogError("Failed to select data: {}", query.lastError().text().toStdString());
        }

        pool_->releaseConnection(connect);
        return jsVal;
    }

    bool checkRecordExist(const std::string &tableName, const std::string &columnName, const std::string &columnValue)
    {
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
        {
            LogError("Sql poll init failed!");
            return false;
        };
        QSqlQuery query(*connect);
        QString sqlQuery = QString("SELECT COUNT(*) AS count FROM %1 WHERE %2 = :value")
                               .arg(QString::fromStdString(tableName))
                               .arg(QString::fromStdString(columnName));
        query.prepare(sqlQuery);
        query.bindValue(":value", QString::fromStdString(columnValue));

        if (query.exec())
        {
            if (query.next())
            {
                int count = query.value("count").toInt();
                pool_->releaseConnection(connect);
                return count > 0;
            }
        }
        else
        {
            LogError("Failed to check data: {}", query.lastError().text().toStdString());
        }

        pool_->releaseConnection(connect);
        return false;
    }

    QString selectOneData(const std::string &tableName, const std::string &selectItem,
                          const std::string &condition = "", const std::string &orderBy = "") // ok
    {
        Json::Value jsVal;
        QString fieldName = "";
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
        {
            LogError("Sql poll init failed!");
            return fieldName;
        }
        QSqlQuery query(*connect);

        // 构建查询数据的SQL语句
        QString sql = "SELECT " + QString::fromStdString(selectItem) + " FROM " + QString::fromStdString(tableName);
        if (!condition.empty())
        {
            sql += " WHERE " + condition;
        }
        if (!orderBy.empty())
        {
            sql += " ORDER BY " + orderBy;
        }
        qDebug() << "sql statement: " << sql;
        query.prepare(sql);
        if (query.exec())
        {
            while (query.next())
            {
                fieldName = query.value(0).toString();
            }
        }
        else
        {
            LogError("Failed to select data: {}", query.lastError().text().toStdString());
        }

        pool_->releaseConnection(connect);
        return fieldName;
    }
    bool execSql(const QString &sql)
    {
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
        {
            LogError("Sql poll init failed!");
            return false;
        }
        QSqlQuery query(*connect);
        query.prepare(sql);
        if (!query.exec())
        {
            qDebug() << query.lastError().text();
            LogError("query lastError: {}", query.lastError().text().toStdString());
            return false;
        }
        pool_->releaseConnection(connect);
        return true;
    }

  private:
    PgsqlHelper()
    {
    }
    QString connectionName_;
    DBConnectionPool *pool_ = nullptr;
};