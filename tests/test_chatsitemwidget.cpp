#include <gtest/gtest.h>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include "../chatsitemwidget.h"

TEST(ChatsItemWidgetTest, Constructor) {
    int argc = 0;
    char **argv = nullptr;
    QApplication app(argc, argv);

    QString testTitle = "Test Chat Title";
    ChatsItemWidget widget(testTitle);

    // Check if specific UI elements are not null
    EXPECT_NE(widget.findChild<QLabel*>("chatsTitleLabel"), nullptr);
    EXPECT_NE(widget.findChild<QPushButton*>("chatsDeleteButton"), nullptr);

    EXPECT_EQ(widget.getTitle(), testTitle);
}
