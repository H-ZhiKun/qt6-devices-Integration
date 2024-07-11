#pragma once
#include <QObject>
#include <QString>
#include <qobject.h>
#include <string>

namespace AppFrame
{
struct TestLogistics
{
    uint32_t number = 0;
    std::string code = "";
};
enum class TestRemoveType
{
    normalRemove = 0,
    allRemove,
    noRemove,
    gapRemove
};
class ExtraTest : public QObject
{
    Q_OBJECT
  public:
    explicit ExtraTest(QObject *parent = nullptr);
    virtual ~ExtraTest() = default;
    // 测试返回物流码
    TestLogistics testLogistics(uint32_t number, const std::string &code);
    void testS7Client();
    bool isPrintTest() const;
    void setPrintTest(bool flag);
    bool isTestRemove() const;
    bool getPosRemoveNeeded();
    void setTestRemove(TestRemoveType type);

  private:
    bool printTest_ = false;
    TestRemoveType removeType_ = TestRemoveType::normalRemove;
    bool lastPosRemove_ = false;
};

} // namespace AppFrame