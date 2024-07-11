#pragma once

#include <QMutex>
#include <QMutexLocker>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QWaitCondition>
#include <cstddef>
#include <memory>
#include <mutex>

class DBConnectionPool
{
  public:
    explicit DBConnectionPool(QString host, quint16 port, QString dbName, QString user, QString password,
                              int maxConnections = 20);
    virtual ~DBConnectionPool();
    size_t getCount();
    std::unique_ptr<QSqlDatabase> getConnection();
    void releaseConnection(std::unique_ptr<QSqlDatabase> &&db);

  protected:
    virtual std::unique_ptr<QSqlDatabase> createConnection();
    std::list<std::unique_ptr<QSqlDatabase>> connectionPool_;
    std::mutex mutex_;
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
