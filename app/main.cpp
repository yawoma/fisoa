#include "utils/logger.h"
#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Test the app for " << argc << " arguments that will be ignored" << '\n';
    for (int i = 0; i < argc; ++i) {
        std::cout << "Argument " << i << ": " << argv[i] << '\n';
    }
    auto logger = flog::Logger::getInstance();
    logger.set_level(flog::Level::DEBUG); // Set log level to INFO
    logger.trace("This is a trace message and should not appear.");
    logger.debug("This is a debug message and should appear.");
    logger.info("This is an info message and should appear.");
    logger.warn("This is a warning message and should appear.");
    logger.error("This is an error message and should appear.");
    logger.critical("This is a critical message and should appear.");
    return 0;
}