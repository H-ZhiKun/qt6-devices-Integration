#include "Cognex.h"
#include "Logger.h"
#include <atomic>

Cognex::Cognex(QObject *parent)
{
}

void Cognex::dealing(std::vector<unsigned char> buffer)
{
    std::string str(buffer.begin(), buffer.end());
    LogInfo("Cognex recv: {}", str);
    size_t commaPos = str.find_first_of(',');
    std::string code = "no read";
    recvCount_.fetch_add(1, std::memory_order_release);
    if (commaPos != std::string::npos)
    {
        code = str.substr(commaPos + 1); // 添加1来跳过逗号
    }
    emit readQRCode(recvCount_.load(std::memory_order_acquire), code);
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