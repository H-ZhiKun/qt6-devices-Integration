#include "MysqlConnectionPool.h"

QSqlDatabase *MysqlConnectionPool::createConnection()
{
    QSqlDatabase *db = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));
    db->setHostName("127.0.0.1");
    db->setPort(3306);
    db->setDatabaseName("integration");
    db->setUserName("root");
    db->setPassword("123456");

    if (db->open())
    {
        return db;
    }
    else
    {
        delete db;
        return nullptr;
    }
}
