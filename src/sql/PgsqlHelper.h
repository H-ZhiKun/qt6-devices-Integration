#pragma once
#include "Logger.h"
#include "NonCopyable.h"
#include "PgsqlConnectionPool.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QList>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <json/json.h>
#include <memory>

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
    }
    bool initSqlHelper(const std::string &host, uint16_t port, const std::string &dbName, const std::string &user,
                       const std::string &password)
    {
        pool_ = std::make_unique<PgsqlConnectionPool>(host.c_str(), port, dbName.c_str(), user.c_str(),
                                                      password.c_str(), 20);
        return pool_->getCount() > 0;
    }
    bool createTable(const std::string &tableName, std::list<std::string> &&fields) // ok
    {
        // 构建创建表的SQL语句
        QString sql = fmt::format("CREATE TABLE IF NOT EXISTS \"{}\" (", tableName.c_str()).c_str();
        for (const auto &field : fields)
        {
            sql += field.c_str() + QString(", ");
        }
        sql.chop(2); // 去除最后的逗号和空格
        sql += ");";
        QList<QVariantMap> sels;
        if (!executeSql(sels, std::move(sql)))
        {
            LogError("Failed to create table {}.", tableName);
            return false;
        }
        return true;
    }

    bool insertData(const std::string &tableName, const QVariantMap &data)
    {
        if (data.size() == 0)
        {
            LogError("{} insert data empty.", tableName);
            return false;
        }
        QString sqlQuery = "INSERT INTO " + QString::fromStdString(tableName) + " (";
        QString placeholders;
        QStringList keys;
        QVariantList vals;
        for (const QString &key : data.keys())
        {
            if (data[key].isNull() || !data[key].isValid() || data[key] == "")
            {
                continue; // 跳过空值字段
            }
            keys << key;
            placeholders += "?,";
            vals.append(data[key]);
        }

        sqlQuery += keys.join(", ") + ") VALUES (" + placeholders.left(placeholders.length() - 1) + ");";
        // LogInfo("insert data: {}", sqlQuery.toStdString());
        QList<QVariantMap> sels;
        if (!executeSql(sels, std::move(sqlQuery), std::move(vals)))
        {
            LogError("{} Failed to insert data.", tableName);
            return false;
        }
        return true;
    }

    bool insertMultipleData(const std::string &tableName, const QList<QVariantMap> &dataList)
    {
        if (dataList.isEmpty())
        {
            LogError("{} insert data is empty.", tableName);
            return false;
        }

        const QVariantMap &sampleData = dataList.first();
        QStringList keys = sampleData.keys();
        if (keys.isEmpty())
        {
            LogError("{} sample data has no keys.", tableName);
            return false;
        }

        // Construct the SQL query
        QString sqlQuery = "INSERT INTO " + QString::fromStdString(tableName) + " (";
        sqlQuery += keys.join(", ") + ") VALUES ";
        QStringList valuePlaceholders;
        QVariantList vals;
        for (const QVariantMap &data : dataList)
        {
            QStringList rowValues;

            for (const QString &key : keys)
            {
                rowValues.append("?");
                vals.append(data.value(key));
            }

            valuePlaceholders.append("(" + rowValues.join(", ") + ")");
        }

        sqlQuery += valuePlaceholders.join(", ");
        sqlQuery += ";";
        // qDebug() << "Insert SQL: " << sqlQuery;

        // Execute the SQL query
        QList<QVariantMap> result;
        {
            if (!executeSql(result, std::move(sqlQuery), std::move(vals), false))
                LogError("{} Failed to insert data.", tableName);
            return false;
        }

        return true;
    }

    bool updateData(const std::string &tableName, const QVariantMap &data, const std::string &&condition)
    {
        if (data.size() == 0)
        {
            LogError("{} update data empty.", tableName);
            return false;
        }
        // 构建更新数据的SQL语句
        QStringList keys;
        QVariantList vals;
        QString sqlQuery = "UPDATE " + QString::fromStdString(tableName) + " SET ";
        for (const QString &key : data.keys())
        {
            if (data[key].isNull() || !data[key].isValid() || data[key] == "")
            {
                continue; // 跳过空值字段
            }
            keys << key;
            vals.append(data[key]);
        }
        keys << "updated_time";
        vals.append(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"));
        sqlQuery += keys.join(" = ?, ");
        sqlQuery += " = ?";
        sqlQuery += QString(" WHERE ") + condition.c_str();
        // qDebug() << "sql update" << sqlQuery;
        QList<QVariantMap> sels;
        if (!executeSql(sels, std::move(sqlQuery), std::move(vals)))
        {
            LogError("Failed to update data {}", tableName);
            return false;
        }
        return true;
    }

    bool upsertData(const std::string &tableName, const QVariantMap &data, const std::string &uniqueColumn)
    {
        if (data.size() == 0)
        {
            LogError("{} upsert data empty.", tableName);
            return false;
        }

        QStringList keys;
        QVariantList vals;
        // 构建插入或更新数据的 SQL 语句
        QString sqlQuery = "INSERT INTO " + QString::fromStdString(tableName) + " (";
        QString updateQuery = " ON CONFLICT (" + QString::fromStdString(uniqueColumn) + ") DO UPDATE SET ";

        for (const QString &key : data.keys())
        {
            if (data[key].isNull() || !data[key].isValid() || data[key] == "")
            {
                continue; // 跳过空值字段
            }
            keys << key;
            vals.append(data[key]);
        }

        sqlQuery += keys.join(", ") + ") VALUES (";
        updateQuery += keys.join(" = EXCLUDED.") + ", ";

        sqlQuery += QString("?)");
        updateQuery.chop(2); // 去除最后的逗号和空格

        sqlQuery += updateQuery;
        QList<QVariantMap> sels;
        if (!executeSql(sels, std::move(sqlQuery), std::move(vals)))
        {
            LogError("Failed to upsert data {}", tableName);
            return false;
        }
        return true;
    }

    bool deleteData(const std::string &tableName, const std::string &condition) // ok
    {
        // 构建删除数据的SQL语句
        QString sql = "DELETE FROM " + QString::fromStdString(tableName) + " WHERE " + condition.c_str();
        QList<QVariantMap> sels;
        if (!executeSql(sels, std::move(sql)))
        {
            LogError("Failed to delete data: {}", tableName);
            return false;
        }
        return true;
    }

    Json::Value selectData(const std::string &tableName, const std::string &&condition = "",
                           const std::string &&orderBy = "")
    {
        Json::Value jsValue;
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
        QList<QVariantMap> sels;
        if (!executeSql(sels, std::move(sql)))
        {
            LogError("Failed to select data: {}", tableName);
            return jsValue;
        }
        for (const auto &items : sels)
        {
            Json::Value jsItems;
            for (const auto &key : items.keys())
            {
                jsItems[key.toStdString()] = std::move(items[key].toString().toStdString());
            }
            jsValue.append(jsItems);
        }
        return jsValue;
    }

    int selectCount(const std::string &tableName, const std::string &&condition = "")
    {
        // 构建查询数据的SQL语句
        QString sql = QString("SELECT COUNT(*) FROM ") + tableName.c_str();
        int resNum{0};
        if (!condition.empty())
        {
            sql += " WHERE " + condition;
        }
        QList<QVariantMap> sels;
        if (!executeSql(sels, std::move(sql)))
        {
            LogError("Failed to select data: {}", tableName);
            return resNum;
        }
        for (const auto &items : sels)
        {
            for (const auto &key : items.keys())
            {
                resNum = items[key].toInt();
            }
        }
        return resNum;
    }

    Json::Value selectDataPaged(const std::string &tableName, const int pageSize, const int pageNumber,
                                const std::string &&condition = "", const std::string &&orderBy = "")
    {
        Json::Value jsValue;

        // 计算要查询的起始行和偏移量
        int offset = (pageNumber - 1) * pageSize;

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

        QList<QVariantMap> sels;
        if (!executeSql(sels, std::move(sql)))
        {
            LogError("Failed to select data: {}", tableName);
            return jsValue;
        }
        for (const auto &items : sels)
        {
            Json::Value jsItems;
            for (const auto &key : items.keys())
            {
                jsItems[key.toStdString()] = std::move(items[key].toString().toStdString());
            }
            jsValue.append(jsItems);
        }
        return jsValue;
    }

    bool checkRecordExist(const std::string &tableName, const std::string &columnName, const std::string &columnValue)
    {

        QString sqlQuery = QString("SELECT COUNT(*) AS count FROM %1 WHERE %2 = %3")
                               .arg(QString::fromStdString(tableName))
                               .arg(QString::fromStdString(columnName))
                               .arg(QString::fromStdString(columnValue));

        QList<QVariantMap> sels;
        if (!executeSql(sels, std::move(sqlQuery)))
        {
            LogError("Failed to check data: {}", tableName);
            return false;
        }
        for (const auto &items : sels)
        {
            for (const auto &key : items.keys())
            {
                return items[key].toInt() > 0;
            }
        }
        return false;
    }

    QString selectOneData(const std::string &tableName, const std::string &selectItem,
                          const std::string &condition = "", const std::string &orderBy = "") // ok
    {

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
        QList<QVariantMap> sels;
        if (!executeSql(sels, std::move(sql)))
        {
            LogError("Failed to select one data: {}", tableName);
            return {};
        }
        for (const auto &items : sels)
        {
            for (const auto &key : items.keys())
            {
                return items[key].toString();
            }
        }
        return {};
    }

  protected:
    bool executeSql(QList<QVariantMap> &selectResults, const QString &&sql, const QVariantList &&vals = QVariantList(),
                    bool isBatch = false)
    {
        bool ret = true;
        auto connect = pool_->getConnection();
        if (connect == nullptr)
        {
            LogError("database poll get failed!");
            return false;
        }
        try
        {
            QSqlQuery query(*connect);
            query.prepare(sql);
            for (const auto &val : vals)
            {
                query.addBindValue(val);
            }
            bool execStatus = false;
            if (isBatch)
            {
                execStatus = query.execBatch();
            }
            else
            {
                execStatus = query.exec();
            }
            if (execStatus)
            {
                if (query.isSelect())
                {
                    // 这是一个 SELECT 查询，可以处理结果集
                    while (query.next())
                    {
                        // 处理查询结果
                        QVariantMap mapData;
                        const QSqlRecord &record = query.record();
                        for (int i = 0; i < record.count(); ++i)
                        {
                            QString fieldName = record.fieldName(i);
                            mapData[record.fieldName(i)] = record.value(i);
                        }
                        selectResults.append(mapData);
                    }
                }
                else
                {
                    // 这不是一个 SELECT 查询，可能是 INSERT、UPDATE、DELETE 等
                    // 执行相应的错误处理或其他操作
                }
            }
            else
            {
                qDebug() << "query lastError: " << query.lastError().text();
                LogError("query lastError: {}", query.lastError().text().toStdString());
                ret = false;
            }
        }
        catch (const std::exception &ex)
        {
            LogError("query exception caught: {}", ex.what());
            ret = false;
        }
        pool_->releaseConnection(std::move(connect));
        return ret;
    }

  private:
    PgsqlHelper()
    {
    }
    QString connectionName_;
    std::unique_ptr<DBConnectionPool> pool_ = nullptr;
};