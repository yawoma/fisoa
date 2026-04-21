#ifndef LOGGER_H
#define LOGGER_H

#include <cstdint>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <string>

/*Logger wrapper based on spdlog*/
namespace flog
{
///! Log levels for the logger
enum class Level : std::uint8_t
{
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class Logger
{
public:
    static Logger& getInstance();
    ~Logger()                        = default;
    Logger(const Logger&)            = delete;    ///< To prevent copy
    Logger& operator=(const Logger&) = delete;    ///< To prevent copy assignment
    Logger(Logger&&)                 = delete;    ///< To prevent move
    Logger& operator=(Logger&&)      = delete;    ///< To prevent move assignment

    void    set_log_file_path(const std::string& logFilePath);
    void    set_level(Level level);

    [[nodiscard]] std::string get_logger_name() const;

    void                      trace(const std::string& msg);
    void                      debug(const std::string& msg);
    void                      info(const std::string& msg);
    void                      warn(const std::string& msg);
    void                      error(const std::string& msg);
    void                      critical(const std::string& msg);

private:
    Logger();
    void                            init();

    ///! The actual spdlog logger instance
    std::shared_ptr<spdlog::logger> m_logger;
    std::string                     m_logFilePath;
    std::string                     m_loggerName;
};
}    // namespace flog

/* Global function to access the logger instance */
flog::Logger& get_logger();

// Macro definitions for logging with file and line information
#define FLOG_TRACE(...)                                                            \
    SPDLOG_LOGGER_TRACE(                                                           \
        spdlog::get(flog::get_logger().get_logger_name()), "[{}:{}] {}", __FILE__, \
        __LINE__, ##__VA_ARGS__)
#define FLOG_DEBUG(...)                                                            \
    SPDLOG_LOGGER_DEBUG(                                                           \
        spdlog::get(flog::get_logger().get_logger_name()), "[{}:{}] {}", __FILE__, \
        __LINE__, ##__VA_ARGS__)
#define FLOG_INFO(...)                                                             \
    SPDLOG_LOGGER_INFO(                                                            \
        spdlog::get(flog::get_logger().get_logger_name()), "[{}:{}] {}", __FILE__, \
        __LINE__, ##__VA_ARGS__)
#define FLOG_WARN(...)                                                             \
    SPDLOG_LOGGER_WARN(                                                            \
        spdlog::get(flog::get_logger().get_logger_name()), "[{}:{}] {}", __FILE__, \
        __LINE__, ##__VA_ARGS__)
#define FLOG_ERROR(...)                                                            \
    SPDLOG_LOGGER_ERROR(                                                           \
        spdlog::get(flog::get_logger().get_logger_name()), "[{}:{}] {}", __FILE__, \
        __LINE__, ##__VA_ARGS__)
#define FLOG_CRITICAL(...)                                                         \
    SPDLOG_LOGGER_CRITICAL(                                                        \
        spdlog::get(flog::get_logger().get_logger_name()), "[{}:{}] {}", __FILE__, \
        __LINE__, ##__VA_ARGS__)

#endif    // LOGGER_H
