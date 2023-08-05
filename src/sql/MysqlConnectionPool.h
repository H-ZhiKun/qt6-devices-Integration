#pragma once
#include "DBConnectionPool.h"
#include <QSqlDatabase>

class MysqlConnectionPool : public DBConnectionPool
{
  public:
    explicit MysqlConnectionPool(int maxConnections = 5, int idleTimeout = 5000)
        : DBConnectionPool(maxConnections, idleTimeout)
    {
        for (uint16_t i = 0; i < 5; i++)
        {
            QSqlDatabase *db = createConnection();
            if (db)
                connectionPool_.append(db);
        }
    }
    virtual ~MysqlConnectionPool()
    {
    }

  public:
    QSqlDatabase *createConnection() override;
};
