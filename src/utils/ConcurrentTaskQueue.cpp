#include "ConcurrentTaskQueue.h"
#include <Logger.h>
#include <assert.h>
#include <fmt/core.h>
#include <iostream>

ConcurrentTaskQueue::ConcurrentTaskQueue()
{
    queueCount_ = std::thread::hardware_concurrency();
    std::cout << "current hardware concurrency = " << queueCount_ << std::endl;
    assert(queueCount_ > 0);
    execQueue_.resize(queueCount_);
    for (uint16_t i = 0; i < queueCount_; ++i)
    {
        threads_.emplace_back(std::thread(std::bind(&ConcurrentTaskQueue::queueFunc, this, i)));
    }
    isInvalid_ = true;
}

void ConcurrentTaskQueue::runTaskInQueue(const std::function<void()> &task)
{
    std::lock_guard<std::mutex> lock(taskMutex_);
    taskQueue_.push(task);
    taskCond_.notify_one();
}
void ConcurrentTaskQueue::runTaskInQueue(std::function<void()> &&task)
{
    std::lock_guard<std::mutex> lock(taskMutex_);
    taskQueue_.push(std::move(task));
    taskCond_.notify_one();
}
void ConcurrentTaskQueue::queueFunc(const uint16_t queueNum)
{
    while (!stop_)
    {
        std::function<void()> r;
        {
            std::unique_lock<std::mutex> lock(taskMutex_);
            while (!stop_ && taskQueue_.size() == 0)
            {
                taskCond_.wait(lock);
            }
            if (!taskQueue_.empty())
            {
                r = std::move(taskQueue_.front());
                taskQueue_.pop();
            }
            else
            {
                continue;
            }
        }
        execQueue_[queueNum] = 1;
        auto inTaskCount = std::count_if(execQueue_.begin(), execQueue_.end(), [](uint8_t val) { return val == 1; });
        std::cout << fmt::format("thread resource = {},thread in task = {},task queue = {}", queueCount_, inTaskCount,
                                 taskQueue_.size())
                  << std::endl;
        r();
        execQueue_[queueNum] = 0;
    }
}

size_t ConcurrentTaskQueue::getTaskCount()
{
    std::lock_guard<std::mutex> guard(taskMutex_);
    return taskQueue_.size();
}

void ConcurrentTaskQueue::stop()
{
    if (!stop_)
    {
        stop_ = true;
        taskCond_.notify_all();
        for (auto &t : threads_)
            t.join();
    }
}
ConcurrentTaskQueue::~ConcurrentTaskQueue()
{
    isInvalid_ = false;
    stop();
}
