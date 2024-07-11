#include "PgsqlConnectionPool.h"
#include "Logger.h"

std::unique_ptr<QSqlDatabase> PgsqlConnectionPool::createConnection()
{

    QString name = QString("pgsql_connection_%1").arg(count_++);
    auto db = std::make_unique<QSqlDatabase>(QSqlDatabase::addDatabase("QPSQL", name));
    db->setHostName(host_);
    db->setPort(port_);
    db->setDatabaseName(dbName_);
    db->setUserName(user_);
    db->setPassword(password_);
    if (!db->open())
    {
        QSqlError error = db->lastError();
        db.reset();
        QSqlDatabase::removeDatabase(name);
        LogError("create sql connection error,{}", error.text().toStdString());
        return {};
    }
    LogInfo("create sql connection size={}", count_);
    return db;
}
