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
    QString getTitle() const;

signals:
    void editClicked();
    void deleteClicked();

private:
    Ui::ChatsItemWidget *ui;
};

#endif // CHATSITEMWIDGET_H
