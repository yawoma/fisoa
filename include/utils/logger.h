#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/logger.h"

/*Logger wrapper based on spdlog*/
namespace flog {
    class Logger{
        public:
        static Logger* getInstance();
        ~Logger();

        private:
        Logger();
    };
} // namespace logger
#endif // LOGGER_H