#pragma once
#include "DBConnectionPool.h"
#include <QSqlDatabase>

class MysqlConnectionPool : public DBConnectionPool
{
  public:
    explicit MysqlConnectionPool(QString host, quint16 port, QString dbName, QString user, QString password,
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
    virtual ~MysqlConnectionPool()
    {
    }

  public:
    QSqlDatabase *createConnection() override;
};
