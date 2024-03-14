#ifndef PRINTERDEVICE_H
#define PRINTERDEVICE_H
#include "TCPClient.h"
#include <list>
#include <vector>

class PrinterDevice : public TCPClient
{
    Q_OBJECT
  public:
    explicit PrinterDevice(QObject *parent = nullptr);
    virtual ~PrinterDevice();
    virtual void dealing(std::vector<unsigned char> buffer) override;
    virtual void sendOnceWhenConnected() override;
    void resetLvPrintCodes();
  public slots:
    void pushCode(std::string code);
  signals:
    void printComplete();
    void printSend();

  private:
    void doStart();                               // 启动指令
    void doPrint();                               // 发送缓存到打印机
    std::string cmdTail_{"\r\n"};                 // 指令结束符
    std::string cmdPrinting_{"SETMSG 2 1"};       // 设置正在打印信号
    std::string cmdFinishPrint_{"SETMSG 3 1"};    // 设置打印完成信号
    std::string cmdStateChange_{"SETMSG 5 1"};    // 启用激光机状态变化自动回馈
    std::string cmdCompile_{"SETMSG 26 1"};       // 设置接收编译完成信号
    std::string cmdBufferWarning_{"SETMSG 27 1"}; // 设置缓存报警
    std::string cmdBufferCover_{"SETMSG 31 1"};   // 设置未打印，后续的数据覆盖
    std::string cmdBufferClear_{"BUFFERCLEAR"};   // 设置buffer清除信号
    std::string cmdPrintState_{"GETMARKMODE"};    /* 查询激光机是否处于可以打印状态:
                                                             可打印：RESULT GETMARKMODE 1
                                                             不可打印: RESULT GETMARKMODE 0 */

    std::string cmdBufferData_{"BUFFERDATA -1"}; // 使用打印缓存

    std::string cmdResetSystem_{"RESETSYSTEM"}; // 重置系统
    std::string cmdMarkStart_{"MARK START"};    // 启动标刻引擎

    std::list<std::string> lvPrintCodes_; // 待打印物流码缓存队列

    bool needSendBuffer_{true}; // 是否给打印机发送缓存
    std::string awaitCMD_{};
    std::list<std::string> lvStartCMD_;
};

#endif // PRINTERDEVICE_H