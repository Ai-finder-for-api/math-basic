/**
 * @file Logger.hpp
 * @brief Lightweight logging utility for debugging math operations.
 */
#pragma once
#include <iostream>
#include <string>

namespace qmath {

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
private:
    static LogLevel current_level;
public:
    static void set_level(LogLevel level) {
        current_level = level;
    }

    static void log(const std::string& message, LogLevel level = LogLevel::INFO) {
        if (level >= current_level) {
            std::string prefix;
            switch (level) {
                case LogLevel::DEBUG: prefix = "[DEBUG] "; break;
                case LogLevel::INFO: prefix = "[INFO] "; break;
                case LogLevel::WARNING: prefix = "[WARNING] "; break;
                case LogLevel::ERROR: prefix = "[ERROR] "; break;
            }
            std::cout << prefix << message << std::endl;
        }
    }
};

LogLevel Logger::current_level = LogLevel::INFO;

} // namespace qmath
