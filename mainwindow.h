#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chatcreatewindow.h"
#include "string"
#include <QListWidget>
#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QWidget>

class chatsettingswindow;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  Ui::MainWindow *getUi() const { return ui; }

  std::string dir_path();

signals:
  void passChatSettings(const QString &title, const QString &prompt,
                        const QString &model, const QString &rag);

private slots:
  void animateButtonPress();
  void animateButtonRelease();

  void on_sendMessageButton_clicked();

  void addShadow(QWidget *widget, int blur, int offset);

  void on_createChatButton_clicked();

  void on_homeButton_clicked();

  void addMessage(bool isUser, const QString &message);

  void on_chatButton_clicked();

  void handleCreateChat(const QString &title, const QString &prompt,
                        const QString &model, const QString &rag);

  void handleUpdateChat(const QString &title, const QString &prompt,
                        const QString &model, const QString &rag);

  void on_chatslistWidget_itemClicked(QListWidgetItem *item);

  void chatDeleteClicked();

  void on_settingsButton_clicked();

  void on_clearChatButton_clicked();

  void on_settingsChatButton_clicked();

  void on_chatRagButton_clicked();

  void saveWindowState();

  void on_chatInput2Widget_returnPressed();

private:
  Ui::MainWindow *ui;
  ChatCreateWindow *chatCreateWindow = new ChatCreateWindow();
  chatsettingswindow *chatSettingsWindow;
  QString typingMessageHtml = "<div style='margin: 10px; padding: "
                              "15px;'><b><i>loading...</i></b></div>";

protected:
  void closeEvent(QCloseEvent *event) override {
    saveWindowState();
    QMainWindow::closeEvent(event); // Accept the close event
  }
};

#endif // MAINWINDOW_H
