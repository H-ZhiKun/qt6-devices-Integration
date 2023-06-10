#pragma once
#include "DBConnectionPool.h"
#include <QSqlDatabase>

class MysqlConnectionPool : public DBConnectionPool
{
public:
    explicit MysqlConnectionPool(int maxConnections = 5, int idleTimeout = 5000)
        : DBConnectionPool(maxConnections, idleTimeout) {}
    virtual ~MysqlConnectionPool() {}
public:
    QSqlDatabase *createConnection() override;
};
