// tests/test_mainwindow.cpp
#include <gtest/gtest.h>
#include <QApplication>
#include "../mainwindow.h"

TEST(MainWindowTest, Constructor) {
    int argc = 0;
    char **argv = nullptr;
    QApplication app(argc, argv);
    
    MainWindow w;
    EXPECT_NE(w.getUi(), nullptr);
}

TEST(MainWindowTest, Page1ButtonClick) {
    int argc = 0;
    char **argv = nullptr;
    QApplication app(argc, argv);

    MainWindow w;
    
    QMetaObject::invokeMethod(&w, "on_page1Button_clicked", Qt::DirectConnection);
    
    // Add expectations based on what the button click should do
    // For example, if clicking the button changes a label text, check for that
    // Assuming you have a label named label in the UI that should change text
    // EXPECT_EQ(w.getUi()->label->text(), "Expected Text");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}