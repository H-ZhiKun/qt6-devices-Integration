#pragma once
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <memory>

// spd 带行号的打印，同时输出console和文件
#define LOGDEBUG(...)                                                                                                  \
    SPDLOG_LOGGER_DEBUG(spdlog::default_logger_raw(), __VA_ARGS__);                                                    \
    SPDLOG_LOGGER_DEBUG(spdlog::get("daily_logger"), __VA_ARGS__)
#define LOGINFO(...)                                                                                                   \
    SPDLOG_LOGGER_INFO(spdlog::default_logger_raw(), __VA_ARGS__);                                                     \
    SPDLOG_LOGGER_INFO(spdlog::get("daily_logger"), __VA_ARGS__)
#define LOGWARN(...)                                                                                                   \
    SPDLOG_LOGGER_WARN(spdlog::default_logger_raw(), __VA_ARGS__);                                                     \
    SPDLOG_LOGGER_WARN(spdlog::get("daily_logger"), __VA_ARGS__)
#define LOGERROR(...)                                                                                                  \
    SPDLOG_LOGGER_ERROR(spdlog::default_logger_raw(), __VA_ARGS__);                                                    \
    SPDLOG_LOGGER_ERROR(spdlog::get("daily_logger"), __VA_ARGS__)

class Logger
{
  public:
    static void InitLogger(std::string logPath);
    static void CloseLogger();
};
