#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/logger.h"
#include <cstdint>
#include <string>

/*Logger wrapper based on spdlog*/
namespace flog {
    ///! Log levels for the logger
    enum class Level : std::uint8_t {
        TRACE,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };

    class Logger{
        public:
        static Logger& getInstance();
        // ~Logger();
        // Logger(const Logger&) = delete; ///< To prevent copy
        // Logger& operator=(const Logger&) = delete; ///< To prevent copy assignment
        // Logger(Logger&&) = delete; ///< To prevent move
        // Logger& operator=(Logger&&) = delete; ///< To prevent move assignment

        void    set_level(Level level);
        void trace(const std::string& msg);
        void debug(const std::string& msg);
        void info(const std::string& msg);
        void warn(const std::string& msg);
        void error(const std::string& msg);
        void critical(const std::string& msg);

        private:
        Logger();
        void init();

        ///! The actual spdlog logger instance
        std::shared_ptr<spdlog::logger> m_Logger;
    };
} // namespace logger
#endif // LOGGER_H