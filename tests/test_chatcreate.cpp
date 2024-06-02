#include <gtest/gtest.h>
#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include "../chatcreatewindow.h"

TEST(ChatCreateWindowTest, Constructor) {
    int argc = 0;
    char **argv = nullptr;
    QApplication app(argc, argv);

    ChatCreateWindow window;

    // Check if specific UI elements are not null
    EXPECT_NE(window.findChild<QLabel*>("titleIconLabel"), nullptr);
    EXPECT_NE(window.findChild<QLabel*>("titleLabel"), nullptr);
    EXPECT_NE(window.findChild<QWidget*>("titleSelectWidget"), nullptr);
    EXPECT_NE(window.findChild<QLabel*>("promptIconLabel"), nullptr);
    EXPECT_NE(window.findChild<QLabel*>("promptTitleLabel"), nullptr);
    EXPECT_NE(window.findChild<QWidget*>("promptSelectWidget"), nullptr);
    EXPECT_NE(window.findChild<QLabel*>("modelIconLabel"), nullptr);
    EXPECT_NE(window.findChild<QLabel*>("modelTitleLabel"), nullptr);
    EXPECT_NE(window.findChild<QComboBox*>("modelSelectWidget"), nullptr);
    EXPECT_NE(window.findChild<QPushButton*>("cancelButton"), nullptr);
    EXPECT_NE(window.findChild<QPushButton*>("createButton"), nullptr);
}