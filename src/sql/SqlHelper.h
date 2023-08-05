#pragma once
#include "Logger.h"
#include "MysqlConnectionPool.h"
#include "NonCopyable.h"
#include "json/json.h"
#include <QJsonDocument>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
class SqlHelper : public AppFrame::NonCopyable
{
  public:
    static SqlHelper &getSqlHelper()
    {
        static SqlHelper helper;
        return helper;
    }

    ~SqlHelper()
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
    bool createTable(const QString &tableName, const QStringList &fields)
    {
        bool ret = true;
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
        {
            return false;
        }
        QSqlQuery query(*connect);

        // 构建创建表的SQL语句
        QString sql = "CREATE TABLE IF NOT EXISTS " + tableName + " (";
        for (const QString &field : fields)
        {
            sql += field + ", ";
        }
        sql.chop(2); // 去除最后的逗号和空格
        sql += ")";
        sql += " ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = DYNAMIC;";

        if (!query.exec(sql))
        {
            LogError("Failed to create table: {}", query.lastError().text().toStdString());
            ret = false;
        }
        pool_->releaseConnection(connect);

        return ret;
    }

    bool insertData(const QString &tableName, const QList<QVariantMap> &dataList)
    {
        bool ret = true;
        if (dataList.isEmpty())
        {
            // 数据列表为空，无需处理
            return false;
        }

        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
            return false;

        QSqlQuery query(*connect);

        // 构建插入数据的SQL语句
        QString sql = "INSERT INTO " + tableName + " (";
        QString values;
        QVariantList params;

        for (const QVariantMap &data : dataList)
        {
            if (data.isEmpty())
            {
                // 数据项为空，跳过该项
                continue;
            }
            QString valuePlaceholders;
            for (auto iter = data.begin(); iter != data.end(); ++iter)
            {
                sql += iter.key() + ", ";
                valuePlaceholders += "?, ";
                // params << QString::number(iter.value().toDouble());
                params << iter.value();
            }
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
        if (!query.exec())
        {
            LogError("Failed to insert data: {}", query.lastError().text().toStdString());
            ret = false;
        }
        pool_->releaseConnection(connect);
        return ret;
    }

    bool updateData(const QString &tableName, const QVariantMap &data, const QString &condition)
    {
        bool ret = true;
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
            return false;

        QSqlQuery query(*connect);
        QVariantList params;

        // 构建更新数据的SQL语句
        QString sql = "UPDATE " + tableName + " SET ";
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            sql += it.key() + " = ?, ";
            params << it.value();
        }
        sql.chop(2); // 去除最后的逗号和空格
        sql += " WHERE " + condition;
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

    bool upsertData(const QString &tableName, const QVariantMap &data)
    {
        bool ret = true;
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
            return false;

        QSqlQuery query(*connect);
        QVariantList params;

        // 构建UPSERT语句
        QString upsertSql = "INSERT INTO " + tableName + " (";
        QString updateSql = ") VALUES (";
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            upsertSql += it.key() + ", ";
            updateSql += "?, ";
            params << it.value();
        }
        upsertSql.chop(2); // 去除最后的逗号和空格
        updateSql.chop(2); // 去除最后的逗号和空格
        upsertSql += updateSql + ") ON DUPLICATE KEY UPDATE ";
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            upsertSql += it.key() + " = VALUES(" + it.key() + "), ";
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

    bool deleteData(const QString &tableName, const QString &condition)
    {
        bool ret = true;
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
            return false;

        QSqlQuery query(*connect);

        // 构建删除数据的SQL语句
        QString sql = "DELETE FROM " + tableName + " WHERE " + condition;

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

    Json::Value selectData(const QString &tableName, const QString &condition, const QString &orderBy = "")
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
        QString sql = "SELECT * FROM " + tableName;
        if (!condition.isEmpty())
        {
            sql += " WHERE " + condition;
        }
        if (!orderBy.isEmpty())
        {
            sql += " ORDER BY " + orderBy;
        }
        qDebug() << "sql statement: " << sql;
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

    bool checkRecordExist(const QString &tableName, const QString &columnName, const QVariant &columnValue)
    {
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
        {
            LogError("Sql poll init failed!");
            return false;
        };
        QSqlQuery query(*connect);
        QString sqlQuery = QString("SELECT COUNT(*) AS count FROM %1 WHERE %2 = :value").arg(tableName).arg(columnName);
        query.prepare(sqlQuery);
        query.bindValue(":value", columnValue);

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

    QString selectOneData(const QString &tableName, const QString &selectItem, const QString &condition,
                          const QString &orderBy = "")
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
        QString sql = "SELECT " + selectItem + " FROM " + tableName;
        if (!condition.isEmpty())
        {
            sql += " WHERE " + condition;
        }
        if (!orderBy.isEmpty())
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

  private:
    SqlHelper()
    {
        pool_ = new MysqlConnectionPool(20, 5000);
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