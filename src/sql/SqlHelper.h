#pragma once
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QSqlError>
#include <QSqlDriver>
#include "DBConnectionPool.h"
#include "Logger.h"

class SqlHelper
{
public:
    SqlHelper(DBConnectionPool *pool)
        : pool_(pool) {}
    ~SqlHelper()
    {
        delete pool_;
    }

    bool createTable(const QString &tableName, const QStringList &fields)
    {
        bool ret = false;
        QSqlDatabase *connect = pool_->getConnection();
        if (connect != nullptr)
        {
            QSqlQuery query(*connect);

            // 构建创建表的SQL语句
            QString sql = "CREATE TABLE IF NOT EXISTS " + tableName + " (";
            for (const QString &field : fields)
            {
                sql += field + ", ";
            }
            sql.chop(2); // 去除最后的逗号和空格
            sql += ")";

            if (query.exec(sql))
            {
                ret = true;
            }
            else
            {
                LOGERROR("Failed to create table: {}", query.lastError().text().toStdString());
            }
            pool_->releaseConnection(connect);
        }
        return ret;
    }

    bool insertData(const QString &tableName, const QList<QVariantMap> &dataList)
    {
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
            return false;

        QSqlQuery query(*connect);

        // 构建插入数据的SQL语句
        QString sql = "INSERT INTO " + tableName + " (";
        QString values;
        QVariantList params;

        for (auto it = dataList.begin(); it != dataList.end(); ++it)
        {
            const QVariantMap &data = *it;
            QString valuePlaceholders;
            for (auto iter = data.begin(); iter != data.end(); ++iter)
            {
                sql += iter.key() + ", ";
                valuePlaceholders += "?, ";
                params << iter.value();
            }
            valuePlaceholders.chop(2); // 去除最后的逗号和空格
            values += "(" + valuePlaceholders + "), ";
        }
        sql.chop(2);    // 去除最后的逗号和空格
        values.chop(2); // 去除最后的逗号和空格
        sql += ") VALUES " + values;

        query.prepare(sql);
        for (const QVariant &param : params)
        {
            query.addBindValue(param);
        }

        if (!query.execBatch())
        {
            LOGERROR("Failed to insert data: {}", query.lastError().text().toStdString());
            return false;
        }

        return true;
    }

    bool updateData(const QString &tableName, const QVariantMap &data, const QString &condition)
    {
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
            return false;

        QSqlQuery query(*connect);

        // 构建更新数据的SQL语句
        QString sql = "UPDATE " + tableName + " SET ";
        for (auto it = data.begin(); it != data.end(); ++it)
        {
            sql += it.key() + " = ?, ";
            query.addBindValue(it.value());
        }
        sql.chop(2); // 去除最后的逗号和空格
        sql += " WHERE " + condition;

        query.prepare(sql);

        if (!query.exec())
        {
            LOGERROR("Failed to update data: {}", query.lastError().text().toStdString());
            return false;
        }

        return true;
    }

    bool deleteData(const QString &tableName, const QString &condition)
    {
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
            return false;

        QSqlQuery query(*connect);

        // 构建删除数据的SQL语句
        QString sql = "DELETE FROM " + tableName + " WHERE " + condition;

        query.prepare(sql);

        if (!query.exec())
        {
            LOGERROR("Failed to delete data: {}", query.lastError().text().toStdString());
            return false;
        }

        return true;
    }

    QSqlRecord selectData(const QString &tableName, const QString &condition, const QString &orderBy)
    {
        QSqlRecord record;
        QSqlDatabase *connect = pool_->getConnection();
        if (connect == nullptr)
            return record;

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

        query.prepare(sql);

        if (!query.exec())
        {
            LOGERROR("Failed to select data: {}", query.lastError().text().toStdString());
            return record;
        }

        // 处理结果集
        // QSqlRecord record = query.record();
        // while (query.next())
        // {
        //     for (int i = 0; i < record.count(); ++i)
        //     {
        //         qDebug() << record.fieldName(i) << ":" << query.value(i);
        //     }
        // }

        return record;
    }

private:
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
    if (record.isEmpty()) {
        qDebug() << "No data found";
    } else {
        while (record.next()) {
            qDebug() << "ID:" << record.value("id").toInt()
                     << "Name:" << record.value("name").toString()
                     << "Age:" << record.value("age").toInt();
        }
    }

*/
