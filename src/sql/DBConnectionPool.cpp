#include "DBConnectionPool.h"
#include <mutex>
DBConnectionPool::DBConnectionPool(QString host, quint16 port, QString dbName, QString user, QString password,
                                   int maxConnections)
    : host_(host), port_(port), dbName_(dbName), user_(user), password_(password), maxConnections_(maxConnections)
{
}

DBConnectionPool::~DBConnectionPool()
{
    releaseAllConnections();
}

size_t DBConnectionPool::getCount()
{
    std::lock_guard locker(mutex_);
    return connectionPool_.size();
}

std::unique_ptr<QSqlDatabase> DBConnectionPool::getConnection()
{
    std::lock_guard locker(mutex_);
    if (!connectionPool_.empty())
    {
        auto db = std::move(connectionPool_.front());
        connectionPool_.pop_front();
        return db;
    }
    else
    {
        return createConnection();
    }
}

void DBConnectionPool::releaseConnection(std::unique_ptr<QSqlDatabase> &&db)
{
    std::lock_guard locker(mutex_);
    if (db->isOpen())
        connectionPool_.push_back(std::move(db));
}

std::unique_ptr<QSqlDatabase> DBConnectionPool::createConnection()
{
    return {};
}

void DBConnectionPool::releaseAllConnections()
{
    std::lock_guard locker(mutex_);
    for (auto &db : connectionPool_)
    {
        db->close();
        QSqlDatabase::removeDatabase(db->connectionName());
    }
    connectionPool_.clear();
}