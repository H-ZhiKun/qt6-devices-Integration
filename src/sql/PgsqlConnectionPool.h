#pragma once
#include "DBConnectionPool.h"
#include <QSqlDatabase>

class PgsqlConnectionPool : public DBConnectionPool
{
  public:
    explicit PgsqlConnectionPool(QString host, quint16 port, QString dbName, QString user, QString password,
                                 int maxConnections = 5, int idleTimeout = 5000)
        : DBConnectionPool(host, port, dbName, user, password, maxConnections, idleTimeout)
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
