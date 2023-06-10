#pragma once
#include "NonCopyable.h"
#include <QQmlApplicationEngine>
#include <QObject>
#include "AppMetaFlash.h"

namespace AppFrame
{
	class AppFramework : public NonCopyable
	{
	public:
		virtual ~AppFramework() = default;
		/**
		 * @brief 获取应用程序框架的唯一实例
		 * @details 该函数返回应用程序框架的唯一实例，是单例模式的实现。该函数不接收任何参数。
		 * @return 返回AppFramework类型的引用，表示应用程序框架的唯一实例。
		 */
		static AppFramework &instance();
		virtual int run(QQmlApplicationEngine *engine) = 0;
		virtual std::string getWorkPath() = 0;
		virtual bool dominoConnect(const QString &ip = "", quint16 port = 0) = 0;
		virtual AppMetaFlash *getAppMetaFlash() = 0;
	};
	inline AppFramework &appFramework()
	{
		return AppFramework::instance();
	}
}
