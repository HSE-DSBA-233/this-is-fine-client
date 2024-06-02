#ifndef CHATSITEMWIDGET_H
#define CHATSITEMWIDGET_H

#include <QWidget>
#include <Qlabel>

namespace Ui {
class ChatsItemWidget;
}

class ChatsItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatsItemWidget(const QString &title, QWidget *parent = nullptr);
    ~ChatsItemWidget();

signals:
    void editClicked();
    void deleteClicked();

private:
    Ui::ChatsItemWidget *ui;
    QLabel *chatsTitleLabel;
};

#endif // CHATSITEMWIDGET_H
