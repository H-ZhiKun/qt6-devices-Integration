#pragma once
#include <QObject>
#include <QString>
#include <string>
#define APP_TEST
struct TestLogistics
{
    uint32_t number = 0;
    std::string code = "";
};
namespace AppFrame
{
class AppTest
{
  public:
    static inline AppTest &instance()
    {
        static AppTest test;
        return test;
    }
    virtual ~AppTest() = default;
    // 测试返回物流码
    TestLogistics test_getLogistics(uint32_t number, const std::string &code);
    void test_s7Client();

  private:
    explicit AppTest() = default;
    AppTest(const AppTest &) = delete;
    AppTest &operator=(const AppTest &) = delete;
    AppTest(AppTest &&) noexcept(true) = default;
    AppTest &operator=(AppTest &&) noexcept(true) = default;
};
inline AppTest &appTest()
{
    return AppTest::instance();
}
} // namespace AppFrame