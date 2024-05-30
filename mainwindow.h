#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>


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
    void on_btn_page1_clicked();
    void animateButtonPress();
    void animateButtonRelease();
    void on_btn_page1_back_clicked();
    void on_btn_page2_clicked();
    void on_btn_page2_back_clicked();
    void addShadow(QWidget *widget);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
