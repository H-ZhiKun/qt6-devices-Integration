#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "Logger.h"
#include "TCPClient.h"
#include <string>
#include <vector>

class Algorithm : public TCPClient
{
    Q_OBJECT
  public:
    explicit Algorithm(QObject *parent = nullptr);
    virtual ~Algorithm() noexcept;
    virtual void dealing(std::vector<unsigned char> buffer) override;
    virtual void pingBehavior() override;

  signals:
    void responseReceived(std::string &response);
  public slots:
    void sendJsonData(const std::string &jsonData);
};

#endif // ALGORITHM_H