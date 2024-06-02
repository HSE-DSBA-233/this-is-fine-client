#include "chatsitemwidget.h"
#include "ui_chatsitemwidget.h"

ChatsItemWidget::ChatsItemWidget(const QString &title, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatsItemWidget)
{
    ui->setupUi(this);

    ui->chatsTitleLabel->setText(title);

    // connect(ui->chatsEditButton, &QPushButton::clicked, this, &ChatsItemWidget::editClicked);
    connect(ui->chatsDeleteButton, &QPushButton::clicked, this, &ChatsItemWidget::deleteClicked);
}

ChatsItemWidget::~ChatsItemWidget()
{
    delete ui;
}
