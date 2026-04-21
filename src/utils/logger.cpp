#include "utils/logger.h"

#include <iostream>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <string>
#include <string_view>

constexpr std::string_view FILE_PATTERN_DESIGN{
    "[%Y-%m-%d %H:%M:%S %z] [%^%L%$] [thread %t] %v"};
constexpr std::string_view CONSOLE_PATTERN_DESIGN{"[Fisoa] [%^%L%$] [thread %t] %v"};
constexpr std::string_view LOG_FILE_DIR{"logs/fisoa_app.log"};
constexpr std::string_view LOGGER_NAME{"flogger_sink"};
const int                  FILE_MAX_SIZE =
    1048576 * 2;            ///< set to 2MB the max size of file before rotation
const int MAX_FILES = 3;    ///< maximum number of file to reach before rotation

namespace flog
{
Logger::Logger(): m_LogFilePath(LOG_FILE_DIR.data()), m_LoggerName(LOGGER_NAME.data())
{
    init();
}

Logger::~Logger() = default;

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::set_log_file_path(const std::string& logFilePath)
{
    m_LogFilePath = logFilePath;
    init();    // Reinitialize the logger with the new file path
}

void Logger::set_level(Level level)
{
    switch (level)
    {
        case Level::TRACE:
            m_Logger->set_level(spdlog::level::trace);
            break;
        case Level::DEBUG:
            m_Logger->set_level(spdlog::level::debug);
            break;
        case Level::INFO:
            m_Logger->set_level(spdlog::level::info);
            break;
        case Level::ERROR:
            m_Logger->set_level(spdlog::level::err);
            break;
        case Level::CRITICAL:
            m_Logger->set_level(spdlog::level::critical);
            break;
        default:
            m_Logger->warn(
                "Invalid log level provided: {}. Defaulting to INFO.",
                static_cast<int>(level));
            m_Logger->set_level(spdlog::level::info);
            break;
    }
}

std::string Logger::get_logger_name() const { return m_LoggerName; }

void        Logger::trace(const std::string& msg)
{
    try
    {
        m_Logger->trace(msg);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Logging failed: " << ex.what() << "\n";
    }
}

void Logger::debug(const std::string& msg)
{
    try
    {
        m_Logger->debug(msg);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Logging failed: " << ex.what() << "\n";
    }
}

void Logger::info(const std::string& msg)
{
    try
    {
        m_Logger->info(msg);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Logging failed: " << ex.what() << "\n";
    }
}

void Logger::warn(const std::string& msg)
{
    try
    {
        m_Logger->warn(msg);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Logging failed: " << ex.what() << "\n";
    }
}

void Logger::error(const std::string& msg)
{
    try
    {
        m_Logger->error(msg);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Logging failed: " << ex.what() << "\n";
    }
}

void Logger::critical(const std::string& msg)
{
    try
    {
        m_Logger->critical(msg);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Logging failed: " << ex.what() << "\n";
    }
}

void Logger::init()
{
    try
    {
        auto console_log = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_log->set_pattern(CONSOLE_PATTERN_DESIGN.data());
        console_log->set_level(spdlog::level::info);

        auto max_file_size = FILE_MAX_SIZE;
        auto max_files     = MAX_FILES;

        auto file_log      = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            m_LogFilePath, max_file_size, max_files);
        file_log->set_pattern(FILE_PATTERN_DESIGN.data());
        file_log->set_level(spdlog::level::trace);

        m_Logger = std::make_shared<spdlog::logger>(
            m_LoggerName, spdlog::sinks_init_list{console_log, file_log});

        spdlog::register_logger(
            m_Logger);    // Register the logger with spdlog to enable macro usage
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Log initialization failed: " << ex.what() << "\n";
    }
}
}    // namespace flog

flog::Logger& get_logger() { return flog::Logger::getInstance(); }
