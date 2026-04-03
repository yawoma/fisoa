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
    Logger::Logger(){}
}