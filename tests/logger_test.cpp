#include "utils/logger.h"

#include <gtest/gtest.h>

TEST(LoggerTest, trace) {
    EXPECT_NO_THROW(flog::Logger::getInstance().trace("This is a trace message."));
}