#include "DBConnectionPool.h"
#include "Logger.h"

DBConnectionPool::DBConnectionPool(int maxConnections, int idleTimeout)
    : maxConnections_(maxConnections), idleTimeout_(idleTimeout)
{
}

DBConnectionPool::~DBConnectionPool()
{
    releaseAllConnections();
}

QSqlDatabase* DBConnectionPool::getConnection()
{
    QMutexLocker locker(&mutex_);

    // 如果连接池中有可用连接，则返回
    if (!connectionPool_.isEmpty()) {
        QSqlDatabase* db = connectionPool_.takeFirst();
        if (db && db->isOpen() && db->isValid()) {
            return db;
        } else {
            // 如果连接已关闭或无效，则重新创建连接并验证
            delete db;
            db = createConnection();
            if (db && db->isOpen() && db->isValid()) {
                return db;
            }
        }
    }

    // 如果连接数未达到最大限制，则创建新连接
    if (connectionPool_.size() < maxConnections_) {
        QSqlDatabase* db = createConnection();
        if (db && db->isOpen() && db->isValid()) {
            return db;
        }
    }

    // 等待空闲连接返回
    if (!waitCondition_.wait(&mutex_, idleTimeout_)) {
        LOGERROR("Timed out waiting for database connection");
        return nullptr;
    }

    // 重新尝试获取连接
    return getConnection();
}

void DBConnectionPool::releaseConnection(QSqlDatabase* db)
{
    if (db && db->isOpen() && db->isValid()) 
    {
        QMutexLocker locker(&mutex_);
        connectionPool_.append(db);
        waitCondition_.wakeOne();
    } 
    else 
    {
        delete db;
    }
}

void DBConnectionPool::releaseAllConnections()
{
    QMutexLocker locker(&mutex_);

    for (QSqlDatabase* db : connectionPool_) {
        if (db && db->isOpen()) {
            db->close();
        }
        delete db;
    }

    connectionPool_.clear();
}
