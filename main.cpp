#include "mainwindow.h"
#include "logger_util.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <iostream>
#include <filesystem>

int main(int argc, char *argv[])
{
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    std::filesystem::path logDir("logs");
    if (!std::filesystem::exists(logDir)) {
        std::filesystem::create_directory(logDir);
    }

    auto logger = getLogger();
    spdlog::set_level(spdlog::level::debug);
    logger->info("Application started");

    QApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "This_is_Fine_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            logger->info("Loaded translator for locale: {}", baseName.toStdString());
            break;
        }
    }

    MainWindow w;
    w.show();
    logger->info("MainWindow displayed");

    int result = app.exec();
    logger->info("Application exited with code {}", result);

    return result;
}
