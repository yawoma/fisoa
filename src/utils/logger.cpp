#include "utils/logger.h"

#include "spdlog/common.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/basic_file_sink.h"
// #include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>
#include <string>
#include <string_view>

constexpr std::string_view PATTERN_DESIGN = "[customer] %v";
constexpr std::string_view LOG_FILE_DIR = "logs/fisoa_app.log";
constexpr std::string_view LOGGER_NAME = "flogger_sink";
// const int FILE_MAX_SIZE = 1048576 * 3; ///< set to 3MB the max size of file before rotation
// const int MAX_FILES = 3; ///< maximum number of file to reach before rotation

namespace flog {
    Logger& Logger::getInstance(){
        static Logger instance;
        return instance;
    }

    void Logger::init(){
        auto console_log = std::make_shared<spdlog::sinks::stdout_color_sink_mt>(); // Args &&args...
        console_log->set_pattern(PATTERN_DESIGN.data());

        auto file_log = std::make_shared<spdlog::sinks::basic_file_sink_mt>(LOG_FILE_DIR.data(), true);
        file_log->set_pattern(PATTERN_DESIGN.data());
        
        m_Logger = spdlog::logger(LOGGER_NAME.data(), {console_log, file_log});
    }

    Logger::Logger() : m_Logger(""){ init(); }
}