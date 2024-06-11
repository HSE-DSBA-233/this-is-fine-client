#ifndef CHATCREATEWINDOW_H
#define CHATCREATEWINDOW_H

#include <QDialog>

namespace Ui {
class ChatCreateWindow;
}

class ChatCreateWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChatCreateWindow(QWidget *parent = nullptr);
    ~ChatCreateWindow();

signals:
    void createChat(const QString &title, const QString &prompt, const QString &model, const QString &rag);

private slots:
    void addShadow(QWidget *widget, int blur, int offset);

    void on_createButton_clicked();

    void on_cancelButton_clicked();

    void on_ragButton_clicked();

private:
    Ui::ChatCreateWindow *ui;
};

#endif // CHATCREATEWINDOW_H
