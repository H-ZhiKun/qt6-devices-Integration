#pragma once
#include <QFutureWatcher>
#include <cstdint>
#include <functional>
#include <qobject.h>
#include <qtmetamacros.h>
#include <string>
#include <unordered_map>

class ConcurrentPool : QObject
{
    Q_OBJECT
  public:
    explicit ConcurrentPool();
    virtual ~ConcurrentPool();
    void runAfter(std::string &&funcName, std::function<void(void)> &&task,
                  uint32_t milliseconds = 0); // 在经过毫秒数后执行任务，默认0立即执行
    void runEvery(std::string &&funcName, std::function<void(void)> &&task,
                  uint32_t milliseconds); // 间隔毫秒数后循环执行任务，0为无间隔
    void stop();

  private:
    bool isThreadFull();
    bool isTaskRunning(const std::string &funcName);
    bool isTimerTask(const std::string &funcName);
    void runTask(std::string &&funcName, std::function<void(void)> &&task);
    void removeTask(const std::string &funcName);
    void timerReset(const std::string &funcName);
    uint32_t threadSize_ = 0;
    QFutureWatcher<void> watcher_;
    std::unordered_map<std::string, QTimer *> mapTimers_;
    std::unordered_map<std::string, std::function<void(void)>> mapTimerTasks_;
    std::unordered_map<std::string, std::unique_ptr<QFutureWatcher<void>>> mapTasks_;
};
