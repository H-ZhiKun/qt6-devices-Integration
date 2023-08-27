#include "PgsqlConnectionPool.h"
#include "Logger.h"

QSqlDatabase *PgsqlConnectionPool::createConnection()
{
    QString name = QString("pgsql_connection_") + QString::number(count_);
    QSqlDatabase *db = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL", name)); // 使用 "QPSQL" 代表 PostgreSQL
    db->setHostName("127.0.0.1");
    db->setPort(5432); // PostgreSQL 默认端口号是 5132
    db->setDatabaseName("integration");
    db->setUserName("postgres"); // PostgreSQL 默认用户名是 "postgres"
    db->setPassword("123456");   // 注意使用的密码

    if (!db->open())
    {
        QSqlError error = db->lastError();
        LogError("\r\n Database error:  {}", error.text().toStdString());
        delete db;
        db = nullptr;
    }
    count_++;
    return db;
}
