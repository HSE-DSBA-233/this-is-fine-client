#include "mainwindow.h"
#include "logger_util.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
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
            logger->info("Loaded translator for locale: {}", locale.toStdString());
            break;
        }
    }

    MainWindow w;
    logger->info("MainWindow created");
    w.show();
    logger->info("MainWindow shown");

    int execResult = app.exec();
    logger->info("Application exited with code {}", execResult);

    return execResult;
}
