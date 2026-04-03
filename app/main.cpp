#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "utils/logger.h"
#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Test the app" << '\n';
    flog::trace("Test the file trace log");
    flog::debug("Test the consol debug log");
    // auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    // console_sink->set_level(spdlog::level::warn);
    // console_sink->set_pattern("[]"); // [multi_sink_example] [%^%l%$] %v

    // auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/multisink.txt", true);
    // file_sink->set_level(spdlog::level::trace);

    // spdlog::logger logger("multi_sink", {console_sink, file_sink});
    // logger.set_level(spdlog::level::debug);
    // logger.warn("this should appear in both {} console and file", 2);
    // logger.info("this message should not appear in the console, only in the file");
    return 0;
}