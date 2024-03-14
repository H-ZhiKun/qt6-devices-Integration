#include "AppTest.h"
#include "Logger.h"
#include "snap7.h"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <thread>
TestLogistics AppFrame::AppTest::test_getLogistics(uint32_t number, const std::string &code)
{
    std::string result = code;
    if (code != "no read")
    {
        result.clear();
        // 设置随机种子
        // 获取当前时间的毫秒数
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        unsigned int seed = milliseconds.count();
        std::srand(seed);

        for (int i = 0; i < 24; ++i)
        {
            // 生成随机数，决定是大写字母还是数字
            int choice = std::rand() % 2;

            if (choice == 0)
            {
                // 生成大写字母的 ASCII 值
                char randomChar = 'A' + std::rand() % 26;
                result += randomChar;
            }
            else
            {
                // 生成数字的 ASCII 值
                char randomDigit = '0' + std::rand() % 10;
                result += randomDigit;
            }
        }
    }
    return {number, result};
}
void AppFrame::AppTest::test_s7Client()
{
    TS7Client client;
    std::mutex mtx;
    if (client.ConnectTo("192.168.123.90", 0, 0) == 0)
    {
        std::thread th1([this, &client, &mtx] {
            uint8_t readBuffer[158]; // 假设要读取64字节的数据
            while (1)
            {
                // 读取数据
                std::lock_guard lock(mtx);
                auto start = std::chrono::high_resolution_clock::now();
                int result = client.DBRead(2, 0, 158, readBuffer);
                if (result == 0)
                {
                    LogInfo("Data read successfully!");
                }
                else
                {
                    LogInfo("Failed to read data. Error code: {}", result);
                }
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> consumption = end - start;
                LogInfo("Data read =  {}", consumption.count());
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        });
        std::thread th2([this, &client, &mtx] {
            uint8_t writeBuffer = 1;
            uint16_t index = 0;
            uint8_t data = 1;
            while (1)
            {
                std::lock_guard lock(mtx);
                auto start = std::chrono::high_resolution_clock::now();
                int result = client.DBWrite(64, 0, 1, &writeBuffer);

                if (result == 0)
                {
                    LogInfo("Data write successfully!");
                }
                else
                {
                    LogInfo("Failed to write data. Error code: ", result);
                }
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> consumption = end - start;
                LogInfo("Data write = {}", consumption.count());
                index++;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        });
        th1.join();
        th2.join();
    }
    else
    {
        std::cerr << "Failed to connect to PLC" << std::endl;
    }
}