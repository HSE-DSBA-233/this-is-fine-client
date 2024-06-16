#ifndef LOGGER_UTIL_H
#define LOGGER_UTIL_H

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include <QCoreApplication>
#include <QDir>

inline std::shared_ptr<spdlog::logger> getLogger() {
  QDir logDir(QCoreApplication::applicationDirPath() + "/logs");
  if (!logDir.exists()) {
    logDir.mkpath(".");
  }
  QString logFilePath = logDir.filePath("basic-log.txt");
  static std::shared_ptr<spdlog::logger> logger =
      spdlog::basic_logger_mt("basic_logger", logFilePath.toStdString());
  return logger;
}

#endif // LOGGER_UTIL_H