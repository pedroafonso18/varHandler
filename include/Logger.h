#pragma once
#include <memory>

#include "../lib/spdlog/sinks/basic_file_sink.h"

class Logger {
private:
    std::shared_ptr<spdlog::logger> logger_;
    std::string filepath_;

    void setup_logger();
public:
    Logger(const std::string& file);
    ~Logger();

    void info(const std::string& message);
    void error(const std::string& message);
    void debug(const std::string& message);
    void warn(const std::string& message);
};