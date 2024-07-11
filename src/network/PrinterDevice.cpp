#include "PrinterDevice.h"
#include "Logger.h"
#include "Utils.h"
#include <cstdint>
#include <qtmetamacros.h>
#include <string>

PrinterDevice::PrinterDevice(QObject *parent) : TCPClient(parent)
{
    lvStartCMD_.push_back(cmdStateChange_);
    lvStartCMD_.push_back(cmdPrinting_);
    // lvStartCMD_.push_back(cmdFinishPrint_);
    // lvStartCMD_.push_back(cmdCompile_);
    lvStartCMD_.push_back(cmdBufferWarning_);
    lvStartCMD_.push_back(cmdBufferCover_);
    lvStartCMD_.push_back(cmdBufferClear_);
    lvStartCMD_.push_back(cmdMarkStart_);
    // lvStartCMD_.push_back(cmdPrintState_);
}

PrinterDevice::~PrinterDevice()
{
}

void PrinterDevice::dealing(std::vector<unsigned char> buffer)
{
    std::string str(buffer.begin(), buffer.end());
    QString originalString(str.c_str());
    originalString.replace("\r\n", "");
    LogInfo("printer recv: {}", originalString.toStdString());
    if (originalString.contains("OK"))
    {
        doStart();
    }
    if (originalString.contains("RESULT GETMARKMODE 0"))
    {
        sendData(QByteArray::fromStdString(cmdResetSystem_ + cmdTail_));
        lvStartCMD_.push_back(cmdMarkStart_);
        return;
    }
    if (originalString.contains("RESULT GETMARKMODE 1"))
    {
        needSendBuffer_ = true;
        return;
    }
    if (originalString.contains("MSG 2") && !originalString.contains("SETMSG"))
    {
        if (originalString.size() > 9)
        {
            uint16_t msgNum = Utils::countSubstrings(originalString.toStdString(), std::string("MSG 2"));
            while (msgNum > 1)
            {
                needSendBuffer_ = true;
                emit printComplete();
                doPrint();
                msgNum--;
            }
        }
        needSendBuffer_ = true;
        emit printComplete();
    }
    doPrint();
}

void PrinterDevice::sendOnceWhenConnected()
{
    // 第一次连接发送启动指令到打印机
    doStart();
}

void PrinterDevice::pushCode(std::string code)
{
    // std::string testCode = code;
    // testCode[2] = '.';
    // lvPrintCodes_.push_back(testCode);
    lvPrintCodes_.push_back(code);
    doPrint();
    LogInfo("printer push code: {},list size = {}", code, lvPrintCodes_.size());
}

void PrinterDevice::doStart()
{
    if (lvStartCMD_.size() > 0)
    {
        auto cmd = lvStartCMD_.front();
        if (sendData(QByteArray::fromStdString(cmd + cmdTail_)))
        {
            awaitCMD_ = cmd;
            LogInfo("printer start cmd: {}", cmd);
            lvStartCMD_.pop_front();
        }
    }
}

void PrinterDevice::doPrint()
{
    if (needSendBuffer_ && lvPrintCodes_.size() > 0)
    {
        auto code = lvPrintCodes_.front();
        if (code.size() == 24)
        {
            // 使用 substr 分割成两个子串
            uint16_t middle = code.length() / 2;
            std::string firstHalf = code.substr(0, middle);
            std::string secondHalf = code.substr(middle);
            code = cmdBufferData_ + " " + firstHalf + " " + secondHalf + "\r\n";
        }
        else
        {
            code = "BUFFERDATA -1 \" \" \" \"\r\n";
        }
        if (sendData(QByteArray::fromStdString(code)))
        {
            needSendBuffer_ = false;
            lvPrintCodes_.pop_front();
            LogInfo("printer recv: and printer print: {}", code);
            emit printSend();
        }
    }
    else
    {
        LogInfo("printer recv: doPrint failed, needSendBuffer_ :{}, size: {}", needSendBuffer_, lvPrintCodes_.size());
    }
}

void PrinterDevice::resetLvPrintCodes()
{
    lvPrintCodes_.clear();
    sendData(QByteArray::fromStdString(cmdBufferClear_ + cmdTail_));
    needSendBuffer_ = true;
    // sendData(QByteArray::fromStdString(cmdMarkStart_ + cmdTail_));
    LogInfo("clear lvPrintCodes_ and size: {}", lvPrintCodes_.size());
}