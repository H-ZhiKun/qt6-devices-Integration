#pragma once
#include "AppFramework.h"
#include "AppMetaFlash.h"
#include "BasePLCDevice.h"
#include "BaseProduct.h"
#include "ConcurrentPool.h"
#include "Utils.h"

namespace AppFrame
{

class AppFrameworkImpl final : public AppFramework
{
  public:
    AppFrameworkImpl();
    ~AppFrameworkImpl() noexcept override;
    inline static AppFrameworkImpl &instance()
    {
        static AppFrameworkImpl instance;
        return instance;
    }
    virtual int run(QQmlApplicationEngine *engine) override;
    virtual QQmlApplicationEngine *getEngine() override;
    virtual QObject *getMeta() override;
    virtual void runAfter(std::string &&taskName, std::function<void(void)> &&task, uint32_t milliseconds = 0) override;
    virtual void runEvery(std::string &&taskName, std::function<void(void)> &&task, uint32_t milliseconds = 0) override;
    virtual std::string expected(const ExpectedFunction &expectedType, const std::string &jsValue) override;
    virtual bool registerExpectation(const ExpectedFunction &expectedType,
                                     std::function<std::string(const std::string &)> &&api) override;
    virtual void quitProgram() override;
    virtual void stop() override;

  protected:
    // 初始化接口区域
    void loadConfig();
    void initFilePath();
    void initLogger();
    void initSqlHelper();
    void initMetaFlash();
    void initAsyncTask();
    void initExtraTest();
    void initNetworkClient();
    void initNetworkServer();
    void initStatistics();
    void initPLC();
    template <typename DeviceType> void registerAndInitPLC();
    void initCameraManager();
    void initProduct();
    template <typename ProductType> void registerAndInitProduct(const YAML::Node &config = YAML::Node());
    void initExpectation();                         // 初始化qml扩展调用模块
    void engineLoad(QQmlApplicationEngine *engine); // 加载engine
    void afterInit();                               // 初始化后行为
    void beforClose();                              // 程序关闭前行为

  private:
    // 私有变量区域
    YAML::Node config_;
    std::unordered_map<ExpectedFunction, std::function<std::string(const std::string &)>> mapExpectedFunction_;
    QQmlApplicationEngine *engine_;
    std::unique_ptr<AppMetaFlash> appMeta_ = {};
    std::unique_ptr<ConcurrentPool> taskActuators_ = {};
};

template <typename DeviceType> inline void AppFrameworkImpl::registerAndInitPLC()
{
    registerPlugin<BasePLCDevice>(std::make_unique<DeviceType>());
    const auto &plcDev = getPlugin<BasePLCDevice>();
    if (!plcDev->init(config_, qApp->applicationDirPath()))
    {
        LogInfo("plc init failed.");
        Utils::appExit(-1);
    }
}

template <typename ProductType> inline void AppFrameworkImpl::registerAndInitProduct(const YAML::Node &config)
{
    registerPlugin<BaseProduct>(std::make_unique<ProductType>());
    const auto &product = getPlugin<BaseProduct>();
    product->init(config);
}

} // namespace AppFrame