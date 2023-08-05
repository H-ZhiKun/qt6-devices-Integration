#include "MysqlConnectionPool.h"
#include "Logger.h"

QSqlDatabase *MysqlConnectionPool::createConnection()
{
    QString name = QString("mysql_connection_") + QString::number(count_);
    QSqlDatabase *db = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", name));
    db->setHostName("127.0.0.1");
    db->setPort(3306);
    db->setDatabaseName("integration");
    db->setUserName("root");
    db->setPassword("123456");

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
