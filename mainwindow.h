#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QListWidget>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_BackMainButton_clicked();
    void on_NewProjectButton_clicked();
    void animateButtonPress();
    void animateButtonRelease();

private:
    Ui::MainWindow *ui;
    QStringList getProjectList();
    void showMainScreen();
    void showProjectListScreen();
};

#endif // MAINWINDOW_H
