#include "PgsqlConnectionPool.h"
#include "Logger.h"

QSqlDatabase *PgsqlConnectionPool::createConnection()
{
    QString name = QString("pgsql_connection_") + QString::number(count_);
    QSqlDatabase *db = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL", name)); // 使用 "QPSQL" 代表 PostgreSQL
    db->setHostName(host_);
    db->setPort(port_); // PostgreSQL 默认端口号是 5132
    db->setDatabaseName(dbName_);
    db->setUserName(user_);     // PostgreSQL 默认用户名是 "postgres"
    db->setPassword(password_); // 注意使用的密码"~!dtfs@#"

    if (!db->open())
    {
        QSqlError error = db->lastError();
        LogError("\r\n Database error:  {}", error.text().toStdString());
        delete db;
        return nullptr;
    }
    count_++;
    return db;
}
