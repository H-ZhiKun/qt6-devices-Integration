#pragma once

#include <QObject>
#include <mutex>

enum class StatisticsType
{
    BaseType = 0,
    CapType,
    LineType,
    CircleType,
    ImpurityType,
    BoxAppType
};

enum class StatisticsKey
{
    amount = 0,
    code_trigger,
    code_get,
    code_right,
    code_error,
    log_feedback,
    log_error,
    log_repeat,
    print_send,
    print_right,
    ocr_trigger,
    ocr_get,
    ocr_right,
    ocr_error,
    locate_trigger,
    locate_get,
    locate_right,
    locate_error,
    check_trigger,
    check_get,
    check_right,
    check_error,
    check_perfect,
    check_good,
    check_commonly,
    check_bad,
    remove,
    all_ng,
    normal
};

enum class ReportKey
{
    amount = 0,    // 成功总数
    faile,         // 失败总数
    qrcodeFaile,   // 二维码读取失败数
    exceptionCode, // 异常码数
    duplicateCode, // 重复码数
    blackFaile,    // 黑名单校验失败
    checkFaile,    // 定位复核失败
    ocrFaile,      // 打码复核失败
    noInner        // 无内塞
};

class BaseStatistics : public QObject
{
    Q_OBJECT

  public:
    explicit BaseStatistics(QObject *parent = nullptr);
    virtual ~BaseStatistics();
    virtual void updateCount(const StatisticsKey key);
    virtual void updateReport(const std::vector<ReportKey> keys);
    bool isContinuousNotGood();
    void setNgMax(uint32_t);
    void reportReset();
    void sendReportEmit();

  protected:
    virtual QVariantMap serialization(const std::unordered_map<StatisticsKey, int32_t> &tempMap) = 0;
    std::chrono::time_point<std::chrono::system_clock> dataClock_ = std::chrono::system_clock::from_time_t(0);
    std::chrono::time_point<std::chrono::system_clock> reportClock_ = std::chrono::system_clock::from_time_t(0);
    uint32_t id_ = 1;
    std::mutex mtxCount_;
    std::mutex mtxReport_;
    std::unordered_map<StatisticsKey, int32_t> recordData_;
    StatisticsType type_ = StatisticsType::BaseType;
    std::atomic<uint32_t> ngCount_ = 0;
    uint32_t ngMax_ = 5;
    std::unordered_map<ReportKey, int32_t> reportData_;

  signals:
    void sendReport(const std::unordered_map<ReportKey, int32_t>);
};
