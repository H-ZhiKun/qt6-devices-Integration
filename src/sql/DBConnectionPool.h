#pragma once

#include <QMutex>
#include <QMutexLocker>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QWaitCondition>

class DBConnectionPool
{
  public:
    explicit DBConnectionPool(int maxConnections = 5, int idleTimeout = 5000);
    virtual ~DBConnectionPool();
    int getCount()
    {
        return count_;
    }
    QSqlDatabase *getConnection();
    void releaseConnection(QSqlDatabase *db);

  public:
    virtual QSqlDatabase *createConnection()
    {
        return nullptr;
    };

  protected:
    QList<QSqlDatabase *> connectionPool_;
    QMutex mutex_;
    int maxConnections_;
    int count_ = 0;
    int idleTimeout_;

    void releaseAllConnections();
};
