#ifndef DOMINO_H
#define DOMINO_H
#include "TCPClient.h"
#include <vector>
class Domino : public TCPClient
{
    Q_OBJECT
public:
    explicit Domino(QObject *parent = nullptr);
    virtual ~Domino() noexcept;
    void dealing(std::vector<unsigned char> buffer) override;
    void pingBehavior() override;
public slots:
    void dominoPrint(std::string code1, std::string code2);
signals:
    void nomal();      // HEX 02 05 01 01 03 激光机正常运作
    void disconnect(); // HEX 02 05 01 02 03 激光器连接不正常
    void printing();   // HEX 02 05 01 03 03  激光机正在标刻
    void printError(); // HEX 02 05 02 02 03 表示发生了打印异常
private:
    void dominoCheck(); // HEX 02 04 01 03 状态查询指令

    std::vector<unsigned char> cmdCheck{0x02, 0x04, 0x01, 0x03};

    // 0x20是初始化填充固定值  命令长度固定，0x02,0x1D,0x02是命令头，03: 表示数据结束结束
    std::vector<unsigned char> cmdPrint{0x02, 0x1D, 0x02,
                                        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                        0x0A,
                                        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                                        0x03};
    enum class ResponseCode
    {
        CodeNomal = 1,
        CodeDisconnect = 2,
        CodePrinting = 3
    };
};

#endif // DOMINO_H