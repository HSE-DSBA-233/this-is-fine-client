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

    void on_page1AboutButton_clicked();
    void on_page1SettingsButton_clicked();
    void on_page1ContactButton_clicked();

    void on_page2Button_clicked();

    void addShadow(QWidget *widget, int blur, int offset);
    
    void on_page3Button_clicked();
    void on_page3BackButton_clicked();
    void on_baseModelButton_clicked();
    void on_ragModelButton_clicked();
    void on_RagChatButton_clicked();
    void on_BackButtonToRAG_clicked();
    void addShadow(QWidget *widget);

    void on_getTokenButton_clicked();

    void on_page4Button_clicked();

    void on_homeButton_clicked();
    void on_page4BackButton_clicked();
    void on_page5BackButton_clicked();
    void on_page6BackButton_clicked();
    void on_page7BackButton_clicked();
    void on_page9BackButton_clicked();

    void addMessage(bool isUser, const QString &message);
    void loadMessageHistory();
    void saveMessageHistory();

    void on_chatButton_clicked();

    void handleCreateChat(const QString &title, const QString &datetime, const QString &model);

    // void animatePageTransition(int newIndex);

    void on_chatslistWidget_itemClicked(QListWidgetItem *item);

    // void chatEditClicked();
    void chatDeleteClicked();

private:
    Ui::MainWindow *ui;
    ChatCreateWindow *chatCreateWindow = new ChatCreateWindow();
};

#endif // MAINWINDOW_H
