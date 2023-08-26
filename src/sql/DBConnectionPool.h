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
    explicit DBConnectionPool(QString host, quint16 port, QString dbName, QString user, QString password,
                              int maxConnections = 5, int idleTimeout = 5000);
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
    QString host_;
    quint16 port_;
    QString dbName_;
    QString user_;
    QString password_;
    void releaseAllConnections();
};
