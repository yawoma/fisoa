#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/logger.h"
#include <string>

namespace flog {
    void set_multi_logger(spdlog::logger& multiLogger);
    void trace(const std::string& message);
    void debug(const std::string& message);
} // namespace logger
#endif // LOGGER_H