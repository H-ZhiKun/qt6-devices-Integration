#pragma once
#include "DBConnectionPool.h"
#include <QSqlDatabase>
#include <cstddef>

class PgsqlConnectionPool : public DBConnectionPool
{
  public:
    explicit PgsqlConnectionPool(QString host, quint16 port, QString dbName, QString user, QString password,
                                 int maxConnections = 5)
        : DBConnectionPool(host, port, dbName, user, password, maxConnections)
    {
        for (size_t i = 0; i < 5; i++)
        {
            auto db = createConnection();
            if (db->isOpen())
                connectionPool_.push_back(std::move(db));
        }
    }
    virtual ~PgsqlConnectionPool()
    {
    }

  public:
    virtual std::unique_ptr<QSqlDatabase> createConnection() override;
};
