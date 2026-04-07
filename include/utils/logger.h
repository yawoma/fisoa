#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/logger.h"
#include <string>

/*Logger wrapper based on spdlog*/
namespace flog {
    enum Level {
        TRACE = 0,
        DEBUG = 1
    };

    class Logger{
        public:
        static Logger& getInstance();
        ~Logger();
        Logger(const Logger&) = delete; ///< To prevent copy
        Logger& operator=(const Logger&) = delete; ///< To prevent assignment
        Logger(Logger&&) = delete; ///< To prevent move
        Logger& operator=(Logger&&) = delete; ///< To prevent move assignment

        void trace(const std::string& msg) const;

        private:
        void init();
        Logger();

        spdlog::logger m_Logger;
    };
} // namespace logger
#endif // LOGGER_H