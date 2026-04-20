#include "utils/logger.h"

#include <gtest/gtest.h>

TEST(LoggerTest, trace_no_throw) {
    EXPECT_NO_THROW(flog::get_logger().trace("This is a trace message"));
}

TEST(LoggerTest, debug_no_throw) {
    EXPECT_NO_THROW(flog::get_logger().debug("This is a debug message"));
}

TEST(LoggerTest, info_no_throw) {
    EXPECT_NO_THROW(flog::get_logger().info("This is an info message"));
}

TEST(LoggerTest, warn_no_throw) {
    EXPECT_NO_THROW(flog::get_logger().warn("This is a warning message"));
}

TEST(LoggerTest, error_no_throw) {
    EXPECT_NO_THROW(flog::get_logger().error("This is an error message"));
}

TEST(LoggerTest, critical_no_throw) {
    EXPECT_NO_THROW(flog::get_logger().critical("This is a critical message"));
}