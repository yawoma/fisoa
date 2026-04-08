#include "utils/logger.h"

#include "spdlog/common.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>
#include <string>
#include <string_view>

constexpr std::string_view FILE_PATTERN_DESIGN{"[%Y-%m-%d %H:%M:%S %z] [%^%L%$] [thread %t] %v"};
constexpr std::string_view CONSOLE_PATTERN_DESIGN{"[Fisoa] [%^%L%$] [thread %t] %v"};
constexpr std::string_view LOG_FILE_DIR{"logs/fisoa_app.log"};
constexpr std::string_view LOGGER_NAME{"flogger_sink"};
const int FILE_MAX_SIZE = 1048576 * 2; ///< set to 2MB the max size of file before rotation
const int MAX_FILES = 3; ///< maximum number of file to reach before rotation

namespace flog {
    Logger::Logger() { init(); }

    // Logger::~Logger() = default;

    Logger& Logger::getInstance(){
        static Logger instance;
        return instance;
    }

    void Logger::set_level(Level level)
    {
        switch (level) {
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

    void Logger::trace(const std::string& msg){
        m_Logger->trace(msg);
    }

    void Logger::debug(const std::string& msg){
        m_Logger->debug(msg);
    }

    void Logger::info(const std::string& msg){
        m_Logger->info(msg);
    }

    void Logger::warn(const std::string& msg){
        m_Logger->warn(msg);
    }

    void Logger::error(const std::string& msg){
        m_Logger->error(msg);
    }

    void Logger::critical(const std::string& msg){
        m_Logger->critical(msg);
    }

    void Logger::init(){
        auto console_log = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_log->set_pattern(CONSOLE_PATTERN_DESIGN.data());
        console_log->set_level(spdlog::level::info);

        auto max_file_size = FILE_MAX_SIZE;
        auto max_files = MAX_FILES;
        auto file_log = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            LOG_FILE_DIR.data(), max_file_size, max_files);
        file_log->set_pattern(FILE_PATTERN_DESIGN.data());
        file_log->set_level(spdlog::level::trace);
        
        std::string logger_name = std::string(LOGGER_NAME.data()); 
        m_Logger = std::make_shared<spdlog::logger>(
            logger_name, spdlog::sinks_init_list{console_log, file_log});
    }
}