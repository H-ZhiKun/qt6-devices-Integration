#pragma once
#include "TaskQueue.h"
#include <cstddef>
#include <cstdint>
#include <list>
#include <queue>
#include <string>
#include <vector>

class ConcurrentTaskQueue : public TaskQueue
{
    struct TaskPackage
    {
        TaskPackage(const std::string &name, const std::function<void()> &task) : taskName_(name), task_(task)
        {
        }
        std::string taskName_;
        std::function<void()> task_;
    };

  public:
    virtual void runTaskInQueue(const std::string &funcName, const std::function<void()> &task);
    virtual void runTaskInQueue(std::string &&funcName, std::function<void()> &&task);

    size_t getTaskCount();
    virtual std::string getName() const
    {
        return "";
    };
    void stop();
    ConcurrentTaskQueue();
    ~ConcurrentTaskQueue();

  private:
    void expansion();
    size_t storeTaskName(const std::string &taskName);
    size_t removeTaskName(const std::string &taskName);
    size_t queueCount_{0};

    std::queue<TaskPackage> taskQueue_;
    std::vector<std::thread> threads_;

    std::mutex taskMutex_;
    std::condition_variable taskCond_;
    std::atomic_bool stop_;
    uint32_t threadId_ = 1;
    void queueFunc(const uint16_t queueNum);
    std::mutex execMutex_;
    std::list<std::string> lvExecution_;
};
