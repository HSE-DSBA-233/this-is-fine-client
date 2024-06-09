#ifndef CHATSETTINGSWINDOW_H
#define CHATSETTINGSWINDOW_H

#include <QDialog>

class MainWindow;

namespace Ui {
class chatsettingswindow;
}

class chatsettingswindow : public QDialog
{
    Q_OBJECT

public:
    explicit chatsettingswindow(MainWindow *parent = nullptr);
    ~chatsettingswindow();

signals:
    void updateChat(const QString &title, const QString &prompt, const QString &model);

private slots:
    void addShadow(QWidget *widget, int blur, int offset);

    void on_submitButton_clicked();

    void on_cancelButton_clicked();

    void handleChatSettings(const QString &title, const QString &prompt, const QString &model);

private:
    Ui::chatsettingswindow *ui;
    MainWindow *mainWindow;
};

#endif // CHATSETTINGSWINDOW_H
