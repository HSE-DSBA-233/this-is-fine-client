#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QListWidget>
#include <QVBoxLayout>
#include "projectswindow.h"

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
    void on_btn_start_clicked();
    void animateButtonPress();
    void animateButtonRelease();

    void on_btn_start_back_clicked();

private:
    Ui::MainWindow *ui;
    ProjectsWindow *projectsWindow;
};

#endif // MAINWINDOW_H
