#pragma once
#include "TaskQueue.h"
#include <cstdint>
#include <queue>
#include <string>
#include <vector>

class ConcurrentTaskQueue : public TaskQueue
{
  public:
    static bool isInvalid()
    {
        return isInvalid_;
    }
    inline static ConcurrentTaskQueue &getInstance()
    {
        static ConcurrentTaskQueue task;
        return task;
    }

    virtual void runTaskInQueue(const std::function<void()> &task);
    virtual void runTaskInQueue(std::function<void()> &&task);

    size_t getTaskCount();
    virtual std::string getName() const
    {
        return "";
    };
    void stop();
    ~ConcurrentTaskQueue();

  private:
    ConcurrentTaskQueue();
    size_t queueCount_{0};

    std::queue<std::function<void()>> taskQueue_;
    std::vector<std::thread> threads_;

    std::mutex taskMutex_;
    std::condition_variable taskCond_;
    std::atomic_bool stop_;
    void queueFunc(const uint16_t queueNum);
    std::vector<uint8_t> execQueue_;
    inline static bool isInvalid_ = true;
};
