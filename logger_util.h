#ifndef LOGGER_UTIL_H
#define LOGGER_UTIL_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

inline std::shared_ptr<spdlog::logger> getLogger() {
    static std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");
    return logger;
}

#endif // LOGGER_UTIL_H
