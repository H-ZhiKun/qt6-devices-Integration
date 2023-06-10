#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>

class DBConnectionPool
{
public:
    explicit DBConnectionPool(int maxConnections = 5, int idleTimeout = 5000);
    virtual ~DBConnectionPool();

    QSqlDatabase* getConnection();
    void releaseConnection(QSqlDatabase* db);
public:
    virtual QSqlDatabase* createConnection() = 0;
protected:
    QList<QSqlDatabase*> connectionPool_;
    QMutex mutex_;
    QWaitCondition waitCondition_;
    int maxConnections_;
    int idleTimeout_;

    void releaseAllConnections();
};
