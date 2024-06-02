#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QString>
#include "chatcreatewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow* getUi() const { return ui; }

private slots:
    void animateButtonPress();
    void animateButtonRelease();

    void on_sendMessageButton_clicked();

    void addShadow(QWidget *widget, int blur, int offset);

    void on_createChatButton_clicked();

    void on_homeButton_clicked();

    void addMessage(bool isUser, const QString &message);

    void loadMessageHistory();
    void saveMessageHistory();

    void on_chatButton_clicked();

    void handleCreateChat(const QString &title, const QString &prompt, const QString &model);

    // void animatePageTransition(int newIndex);

    void on_chatslistWidget_itemClicked(QListWidgetItem *item);

    // void chatEditClicked();
    void chatDeleteClicked();

    void on_settingsUpdateButton_clicked();

    void on_settingsButton_clicked();

private:
    Ui::MainWindow *ui;
    ChatCreateWindow *chatCreateWindow = new ChatCreateWindow();
};

#endif // MAINWINDOW_H
