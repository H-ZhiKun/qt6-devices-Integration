#include "ConcurrentTaskQueue.h"
#include <Logger.h>
#include <assert.h>
#include <cstddef>
#include <fmt/core.h>
#include <iostream>

ConcurrentTaskQueue::ConcurrentTaskQueue()
{
    queueCount_ = std::thread::hardware_concurrency();
    std::cout << "current hardware concurrency = " << queueCount_ << std::endl;
    assert(queueCount_ > 0);
    expansion();
}

void ConcurrentTaskQueue::runTaskInQueue(const std::string &funcName, const std::function<void()> &task)
{
    if (stop_)
        return;
    expansion();
    std::lock_guard<std::mutex> lock(taskMutex_);
    taskQueue_.push(TaskPackage(funcName, task));
    taskCond_.notify_one();
}
void ConcurrentTaskQueue::runTaskInQueue(std::string &&funcName, std::function<void()> &&task)
{
    if (stop_)
        return;
    expansion();
    std::lock_guard<std::mutex> lock(taskMutex_);
    taskQueue_.push(std::move(TaskPackage(funcName, task)));
    taskCond_.notify_one();
}
void ConcurrentTaskQueue::queueFunc(const uint16_t queueNum)
{
    while (!stop_)
    {
        std::function<void()> r;
        std::string funcName;
        {
            std::unique_lock<std::mutex> lock(taskMutex_);
            while (!stop_ && taskQueue_.size() == 0)
            {
                taskCond_.wait(lock);
            }
            if (!taskQueue_.empty())
            {
                auto &package = taskQueue_.front();
                funcName = std::move(package.taskName_);
                r = std::move(package.task_);
                taskQueue_.pop();
            }
            else
            {
                continue;
            }
        }
        storeTaskName(funcName);
        r();
        removeTaskName(funcName);
    }
}

size_t ConcurrentTaskQueue::getTaskCount()
{
    std::lock_guard<std::mutex> guard(taskMutex_);
    return taskQueue_.size();
}

size_t ConcurrentTaskQueue::storeTaskName(const std::string &taskName)
{
    std::lock_guard<std::mutex> lock(execMutex_);
    lvExecution_.emplace_back(std::move(taskName));
    return lvExecution_.size();
}
size_t ConcurrentTaskQueue::removeTaskName(const std::string &taskName)
{
    std::lock_guard<std::mutex> lock(execMutex_);
    lvExecution_.remove_if([&taskName](const std::string &str) { return str == taskName; });
    return lvExecution_.size();
}
void ConcurrentTaskQueue::stop()
{
    if (!stop_)
    {
        stop_ = true;
        taskCond_.notify_all();
        for (auto &t : threads_)
        {
            t.join();
        }
    }
}
ConcurrentTaskQueue::~ConcurrentTaskQueue()
{
    stop();
}

void ConcurrentTaskQueue::expansion()
{
    if (lvExecution_.size() == threads_.size() && threads_.size() <= queueCount_)
    {
        for (size_t i = 0; i < 5; ++i, ++threadId_)
        {
            threads_.emplace_back(std::thread(std::bind(&ConcurrentTaskQueue::queueFunc, this, threadId_)));
            LogInfo("thread created id={}", threadId_);
        }
    }
}