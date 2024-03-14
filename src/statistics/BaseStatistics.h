#pragma once

#include <QObject>

using namespace std::chrono;
enum class StatisticsType
{
    BaseType = 0,
    CapType,
    LineType,
    CircleType
};

enum class StatisticsKey
{
    amount = 0,
    code_trigger,
    code_get,
    code_right,
    code_feedback,
    print_send,
    print_right,
    ocr_trigger,
    ocr_get,
    locate_trigger,
    locate_get,
    locate_right,
    check_trigger,
    check_get,
    check_right,
    remove
};

class BaseStatistics : public QObject
{
    Q_OBJECT

  public:
    explicit BaseStatistics(QObject *parent = nullptr);
    virtual ~BaseStatistics() = default;
    void updateCount(const StatisticsKey key);

  protected:
    QVariantMap serialization();
    time_point<system_clock> dataClock_ = system_clock::from_time_t(0);
    uint32_t id_ = 1;
    std::unordered_map<StatisticsKey, uint32_t> recordData_{
        {StatisticsKey::amount, 0},         {StatisticsKey::code_trigger, 0},  {StatisticsKey::code_get, 0},
        {StatisticsKey::code_right, 0},     {StatisticsKey::code_feedback, 0}, {StatisticsKey::print_send, 0},
        {StatisticsKey::print_right, 0},    {StatisticsKey::ocr_trigger, 0},   {StatisticsKey::ocr_get, 0},
        {StatisticsKey::locate_trigger, 0}, {StatisticsKey::locate_get, 0},    {StatisticsKey::locate_right, 0},
        {StatisticsKey::check_trigger, 0},  {StatisticsKey::check_get, 0},     {StatisticsKey::check_right, 0},
        {StatisticsKey::remove, 0},
    };
    StatisticsType type_ = StatisticsType::BaseType;
};
