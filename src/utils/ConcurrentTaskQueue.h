#pragma once
#include "TaskQueue.h"
#include <list>
#include <memory>
#include <queue>
#include <string>
#include <vector>

class ConcurrentTaskQueue : public TaskQueue
{
  public:
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
    void queueFunc(int queueNum);
};
