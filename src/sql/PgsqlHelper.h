#pragma once
#include "Logger.h"
#include "NonCopyable.h"
#include "PgsqlConnectionPool.h"
#include "json/json.h"
#include <QJsonDocument>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
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
    bool initSqlHelper()
    {
        bool init = true;
        auto db = pool_->getConnection();
        if (db == nullptr)
        {
            init = false;
        }
        else
        {
            pool_->releaseConnection(db);
        }
        return init;
    }
    bool createTable(const std::string &tableName, std::list<std::string> &&fields)
    {
        bool ret = true;
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
        {
            return false;
        }
        QSqlQuery query(*connect);
        QString sql = fmt::format("CREATE TABLE IF NOT EXISTS \"{0}\" (", tableName.c_str()).c_str();
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
    }

    bool insertData(const std::string &tableName, const Json::Value &&jsValue)
    {
        bool ret = true;

        if (jsValue.isNull())
        {
            // 如果 jsValue 是空，无需处理
            return false;
        }

        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
        {
            return false;
        }

        QSqlQuery query(*connect);
        // 判断 jsValue 是数组还是对象
        if (jsValue.isArray())
        {
            // 构建批量插入数据的 SQL 语句
            QString sql = fmt::format("INSERT INTO \"{0}\" (", tableName).c_str();
            QString values;
            QVariantList params;
            bool bAddfield = false;

            for (const Json::Value &data : jsValue)
            {
                if (!data.isObject())
                {
                    // 跳过非对象类型
                    continue;
                }
                QString valuePlaceholders;
                for (auto iter = data.begin(); iter != data.end(); ++iter)
                {
                    if (!bAddfield)
                    {
                        sql += fmt::format("\"{0}\", ", iter.name().c_str()).c_str();
                    }
                    valuePlaceholders += "?, ";
                    QVariant curVal;
                    if (iter->isBool())
                        curVal = iter->asBool();
                    if (iter->isInt())
                        curVal = iter->asInt();
                    if (iter->isDouble())
                        curVal = iter->asFloat();
                    if (iter->isString())
                        curVal = iter->asCString();
                    params << curVal;
                }
                bAddfield = true;
                valuePlaceholders.chop(2); // 去除最后的逗号和空格
                values += "(" + valuePlaceholders + "), ";
            }
            sql.chop(2);    // 去除最后的逗号和空格
            values.chop(2); // 去除最后的逗号和空格
            sql += ") VALUES " + values;
            query.prepare(sql);

            for (int i = 0; i < params.size(); ++i)
            {
                query.addBindValue(params[i].toString());
            }
        }
        else if (jsValue.isObject())
        {
            // 直接插入单个对象
            QString sql = fmt::format("INSERT INTO \"{0}\" (", tableName).c_str();
            QString values;
            QVariantList params;

            QString valuePlaceholders;
            for (auto iter = jsValue.begin(); iter != jsValue.end(); ++iter)
            {
                sql += fmt::format("\"{0}\", ", iter.name().c_str()).c_str();
                valuePlaceholders += "?, ";
                QVariant curVal;
                if (iter->isBool())
                    curVal = iter->asBool();
                if (iter->isInt())
                    curVal = iter->asInt();
                if (iter->isDouble())
                    curVal = iter->asFloat();
                if (iter->isString())
                    curVal = iter->asCString();
                params << curVal;
            }
            valuePlaceholders.chop(2); // 去除最后的逗号和空格
            values += "(" + valuePlaceholders + ")";
            sql.chop(2); // 去除最后的逗号和空格
            sql += ") VALUES " + values;
            query.prepare(sql);

            for (int i = 0; i < params.size(); ++i)
            {
                query.addBindValue(params[i].toString());
            }
        }

        if (!query.exec())
        {
            LogError("Failed to insert data: {}", query.lastError().text().toStdString());
            ret = false;
        }

        pool_->releaseConnection(connect);
        return ret;
    }

    bool updateData(const std::string &tableName, Json::Value &&jsValue, const std::string &&condition)
    {
        bool ret = true;

        if (jsValue.isNull())
        {
            // 如果 jsValue 是空，无需处理
            return false;
        }

        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
        {
            return false;
        }

        QSqlQuery query(*connect);
        QVariantList params;

        // 构建更新数据的SQL语句
        QString sql = "UPDATE " + QString::fromStdString(tableName) + " SET ";
        for (auto it = jsValue.begin(); it != jsValue.end(); ++it)
        {
            sql += QString::fromStdString(it.key().asString()) + " = ?, ";
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

        sql.chop(2); // 去除最后的逗号和空格
        sql += QString(" WHERE ") + condition.c_str();
        query.prepare(sql);
        qDebug() << "sql update: " << sql;
        for (int i = 0; i < params.size(); ++i)
        {
            query.addBindValue(params[i].toString());
        }

        if (!query.exec())
        {
            LogError("Failed to update data: {}", query.lastError().text().toStdString());
            ret = false;
        }
        pool_->releaseConnection(connect);
        return ret;
    }

    bool upsertData(const std::string &tableName, const Json::Value &&data)
    {
        bool ret = true;
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
            return false;

        QSqlQuery query(*connect);
        QVariantList params;

        // 构建UPSERT语句
        QString upsertSql = fmt::format("INSERT INTO \"{0}\" (", tableName.c_str()).c_str();
        QString updateSql = ") VALUES (";
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            upsertSql += "\"" + it.name() + "\", ";
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
        upsertSql += updateSql + ") ON CONFLICT DO UPDATE SET ";
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            upsertSql += "\"" + it.name() + "\" = EXCLUDED.\"" + it.name() + "\", ";
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
        pool_->releaseConnection(connect);
        return ret;
    }

    bool deleteData(const std::string &tableName, const std::string &condition)
    {
        bool ret = true;
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
            return false;

        QSqlQuery query(*connect);

        // 构建删除数据的SQL语句
        QString sql =
            "DELETE FROM \"" + QString::fromStdString(tableName) + "\" WHERE " + QString::fromStdString(condition);

        query.prepare(sql);
        qDebug() << "delete user sql" << sql;
        if (!query.exec())
        {
            LogError("Failed to delete data: {}", query.lastError().text().toStdString());
            ret = false;
        }
        pool_->releaseConnection(connect);
        return ret;
    }

    Json::Value selectData(const std::string &tableName, const std::string &&condition,
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
        QString sql = "SELECT * FROM \"" + QString::fromStdString(tableName) + "\"";
        if (!condition.empty())
        {
            sql += " WHERE " + QString::fromStdString(condition);
        }
        if (!orderBy.empty())
        {
            sql += " ORDER BY " + QString::fromStdString(orderBy);
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
                                const std::string &&condition = "", const std::string &&orderBy = "")
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
            sql += " WHERE " + QString::fromStdString(condition);
        }
        if (!orderBy.empty())
        {
            sql += " ORDER BY " + QString::fromStdString(orderBy);
        }
        // 添加分页限制
        sql += QString(" LIMIT %1 OFFSET %2").arg(pageSize).arg(offset);

        if (query.exec(sql))
        {
            while (query.next())
            {
                QSqlRecord record = query.record();
                Json::Value jsItem;
                for (int i = 0; i < record.count(); ++i)
                {
                    QString fieldName = record.fieldName(i);
                    jsItem[fieldName.toStdString()] = query.value(i).toString().toStdString();
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
            LogError("Sql pool init failed!");
            return false;
        }
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
                          const std::string &condition = "", const std::string &orderBy = "")
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
        QString sql =
            "SELECT \"" + QString::fromStdString(selectItem) + "\" FROM \"" + QString::fromStdString(tableName) + "\"";
        if (!condition.empty())
        {
            sql += " WHERE " + QString::fromStdString(condition);
        }
        if (!orderBy.empty())
        {
            sql += " ORDER BY \"" + QString::fromStdString(orderBy) + "\"";
        }
        qDebug() << "sql statement: " << sql;
        query.prepare(sql);

        if (query.exec())
        {
            if (query.next())
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

  private:
    PgsqlHelper()
    {
        pool_ = new PgsqlConnectionPool(20, 5000);
    }
    QString connectionName_;
    DBConnectionPool *pool_;
};
/*
    //-----------------使用示例------------------//

    // 创建数据库连接池
    继承 DBConnectionPool
    重写 DBConnectionPool createConnection

    创建不同版本的连接池类。
    //注意pool由helper管理释放，不需要手动释放

    // 创建 SqlHelper 对象
    SqlHelper sqlHelper(new YourConnectionPool(5, 5000));

    // 创建表
    QStringList fields;
    fields << "id INT PRIMARY KEY AUTO_INCREMENT"
           << "name VARCHAR(100) NOT NULL"
           << "age INT NOT NULL";
    if (sqlHelper.createTable("students", fields)) {
        qDebug() << "Table created successfully";
    } else {
        qDebug() << "Failed to create table";
    }

    // 插入数据
    QList<QVariantMap> dataList;
    QVariantMap data1;
    data1["name"] = "John";
    data1["age"] = 25;
    dataList.append(data1);

    QVariantMap data2;
    data2["name"] = "Jane";
    data2["age"] = 30;
    dataList.append(data2);

    if (sqlHelper.insertData("students", dataList)) {
        qDebug() << "Data inserted successfully";
    } else {
        qDebug() << "Failed to insert data";
    }

    // 更新数据
    QVariantMap updateData;
    updateData["age"] = 26;
    if (sqlHelper.updateData("students", updateData, "name = 'John'")) {
        qDebug() << "Data updated successfully";
    } else {
        qDebug() << "Failed to update data";
    }

    // 删除数据
    if (sqlHelper.deleteData("students", "name = 'Jane'")) {
        qDebug() << "Data deleted successfully";
    } else {
        qDebug() << "Failed to delete data";
    }

// 查询数据
QSqlRecord record = sqlHelper.selectData("students", "", "name");
if (record.isEmpty())
{
    qDebug() << "No data found";
}
else
{
    while (record.next())
    {
        qDebug() << "ID:" << record.value("id").toInt() << "Name:" << record.value("name").toString()
                 << "Age:" << record.value("age").toInt();
    }
}
*/