#include "Cognex.h"
#include "Logger.h"
#include "Utils.h"
#include <atomic>
#include <string>

Cognex::Cognex(QObject *parent)
{
}

void Cognex::dealing(std::vector<unsigned char> buffer)
{
    std::string str(buffer.begin(), buffer.end());
    LogInfo("Cognex recv all: {}", str);
    auto vRecvData = Utils::splitString(str, "\r\n");
    for (const auto &info : vRecvData)
    {
        std::string code = "no read";
        size_t commaPos = info.find_first_of(',');
        recvCount_.fetch_add(1, std::memory_order_release);
        int recvCount = recvCount_.load(std::memory_order_acquire);
        LogInfo("Cognex recv: {}, qrCode count: {}", info, recvCount);
        if (commaPos != std::string::npos)
        {
            code = info.substr(commaPos + 1); // 添加1来跳过逗号
        }
        emit readQRCode(recvCount, code);
    }
}

Cognex::~Cognex() noexcept
{
}
uint32_t Cognex::getCount()
{
    return recvCount_.load(std::memory_order_acquire);
}
void Cognex::cleanCount()
{
    recvCount_.store(0, std::memory_order_release);
}

void Cognex::resetCount(const uint32_t count)
{
    recvCount_.store(count, std::memory_order_release);
}