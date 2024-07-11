#include "ConcurrentPool.h"
#include <Logger.h>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/qtconcurrentrun.h>
#include <fmt/core.h>
#include <memory>
#include <qobject.h>

ConcurrentPool::ConcurrentPool() : QObject(nullptr)
{
    auto threadPool = QThreadPool::globalInstance();
    threadSize_ = threadPool->maxThreadCount();
}
ConcurrentPool::~ConcurrentPool()
{
    mapTimers_.clear();
}

void ConcurrentPool::runAfter(std::string &&funcName, std::function<void(void)> &&task, uint32_t milliseconds)
{
    if (milliseconds > 0)
    {
        QTimer::singleShot(milliseconds, [this, funcName = std::move(funcName), task = std::move(task)]() mutable {
            runTask(std::move(funcName), std::move(task));
        });
    }
    else
    {
        runTask(std::move(funcName), std::move(task));
    }
}
void ConcurrentPool::runEvery(std::string &&funcName, std::function<void(void)> &&task, uint32_t milliseconds)
{
    if (isTimerTask(funcName))
    {
        LogWarn("async thread timer task Already exists, {}", funcName);
        return;
    }
    QTimer *timer = new QTimer(this);
    mapTimers_.insert({funcName, timer});
    mapTimerTasks_.insert({funcName, task});
    QObject::connect(timer, &QTimer::timeout, [this, timer, funcName]() {
        timer->stop();
        auto name = funcName;
        auto task = mapTimerTasks_.at(funcName);
        runTask(std::move(name), std::move(task));
    });
    timer->setInterval(milliseconds);
    timer->start();
}

void ConcurrentPool::stop()
{
    for (const auto &it : mapTimers_)
    {
        it.second->stop();
    }
}

void ConcurrentPool::runTask(std::string &&funcName, std::function<void(void)> &&task)
{
    // isThreadFull();
    auto future = QtConcurrent::run([this, task = std::move(task)]() -> void { task(); });
    if (!future.isValid())
    {
        LogError("async thread task start error:{}", funcName);
        return;
    }
    std::unique_ptr<QFutureWatcher<void>> watcher = std::make_unique<QFutureWatcher<void>>();
    QObject::connect(watcher.get(), &QFutureWatcher<void>::finished, [this, funcName]() {
        removeTask(funcName);
        timerReset(funcName);
    });
    watcher->setFuture(future);
    mapTasks_[funcName] = std::move(watcher);
}
bool ConcurrentPool::isThreadFull()
{
    if (mapTasks_.size() >= threadSize_)
    {
        std::string taskDes;
        for (const auto &it : mapTasks_)
        {
            if (it.first.empty())
            {
                LogInfo("thread error: name empty");
                continue;
            }
            taskDes += it.first + ",";
        }
        taskDes.erase(taskDes.size() - 1);
        LogError("async thread task filled run:{},max:{},<{}>.", mapTasks_.size(), threadSize_, taskDes);
        return true;
    }
    return false;
}
bool ConcurrentPool::isTaskRunning(const std::string &funcName)
{
    return mapTasks_.find(funcName) != mapTasks_.end();
}
bool ConcurrentPool::isTimerTask(const std::string &funcName)
{
    return mapTimers_.find(funcName) != mapTimers_.end();
}

void ConcurrentPool::removeTask(const std::string &funcName)
{
    auto iter = mapTasks_.find(funcName);
    if (iter != mapTasks_.end())
    {
        const auto &f = iter->second->future();
        // if (f.isFinished() || f.isCanceled())
        if (!f.isValid())
        {
            LogError("async thread task run error:{}", iter->first);
        }
        mapTasks_.erase(iter);
    }
}
void ConcurrentPool::timerReset(const std::string &funcName)
{
    auto iter = mapTimers_.find(funcName);
    if (iter != mapTimers_.end())
    {
        const auto &timer = iter->second;
        timer->start();
    }
}