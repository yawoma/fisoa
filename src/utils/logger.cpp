#include "utils/logger.h"

#include "spdlog/common.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>
#include <string>
#include <string_view>

constexpr std::string_view PATTERN_DESIGN = "[customer] %v";
constexpr std::string_view LOG_FILE_DIR = "logs/fisoa_app.log";
const auto FILE_MAX_SIZE = 1048576 * 3; ///< set to 3MB the max size of file before rotation
const auto MAX_FILES = 3; ///< maximum number of file to reach before rotation

namespace flog {
    void set_multi_logger(spdlog::logger& multiLogger){
        // console loger setting
        auto console_log = std::make_shared<spdlog::sinks::stdout_color_sink_mt>(); //Args &&args...
        console_log->set_level(spdlog::level::debug);
        console_log->set_pattern(PATTERN_DESIGN.data());

        // file logger set
        auto file_log = std::make_shared<spdlog::sinks::basic_file_sink_mt>(LOG_FILE_DIR.data()); //spdlog::sinks::rotating_file_sink_mt
        file_log->set_level(spdlog::level::trace);
        file_log->set_pattern(PATTERN_DESIGN.data());
    
        multiLogger = spdlog::logger("fisoa_multi_sinks", {console_log, file_log});
        // return logger;
    }

    void trace(const std::string &message){
        // spdlog::logger logger("");
        // set_multi_logger(logger);
        // logger.trace(message);
        spdlog::trace(message);
    }

    void debug(const std::string& message){
        spdlog::logger logger("");
        set_multi_logger(logger);
        logger.debug(message);
    }
}