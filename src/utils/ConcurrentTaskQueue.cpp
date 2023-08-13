#include "ConcurrentTaskQueue.h"
#include <assert.h>

ConcurrentTaskQueue::ConcurrentTaskQueue()
{
    queueCount_ = std::thread::hardware_concurrency() / 2;
    assert(queueCount_ > 0);
    for (unsigned int i = 0; i < queueCount_; ++i)
    {
        threads_.emplace_back(std::thread(std::bind(&ConcurrentTaskQueue::queueFunc, this, i)));
    }
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
void ConcurrentTaskQueue::queueFunc(int queueNum)
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
            if (taskQueue_.size() > 0)
            {
                r = std::move(taskQueue_.front());
                taskQueue_.pop();
            }
            else
                continue;
        }
        r();
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
    stop();
}
