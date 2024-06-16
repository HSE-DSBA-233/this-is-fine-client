#include "logger_util.h"
#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QScreen>
#include <QStyle>
#include <QTranslator>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  auto logger = getLogger();
  spdlog::set_level(spdlog::level::debug);

  logger->info("Application started");

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "This_is_Fine_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      app.installTranslator(&translator);
      logger->info("Loaded translator for locale: {}", locale.toStdString());
      break;
    }
  }

  MainWindow window;
  logger->info("MainWindow created");

  QRect availableGeometry =
      QGuiApplication::primaryScreen()->availableGeometry();
  window.move(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
                                  window.size(), availableGeometry)
                  .topLeft());

  window.show();

  logger->info("MainWindow shown");

  int execResult = app.exec();
  logger->info("Application exited with code {}", execResult);

  return execResult;
}
