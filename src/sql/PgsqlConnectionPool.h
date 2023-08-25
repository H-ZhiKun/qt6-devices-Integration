#pragma once
#include "DBConnectionPool.h"
#include <QSqlDatabase>

class PgsqlConnectionPool : public DBConnectionPool
{
  public:
    explicit PgsqlConnectionPool(int maxConnections = 5, int idleTimeout = 5000)
        : DBConnectionPool(maxConnections, idleTimeout)
    {
        for (uint16_t i = 0; i < 5; i++)
        {
            QSqlDatabase *db = createConnection();
            if (db)
                connectionPool_.append(db);
        }
    }
    virtual ~PgsqlConnectionPool()
    {
    }

  public:
    QSqlDatabase *createConnection() override;
};
