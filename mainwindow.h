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
    void animateButtonPress();
    void animateButtonRelease();

    void on_page1Button_clicked();

    void on_page2Button_clicked();
    void on_page2BackButton_clicked();

    void on_page3Button_clicked();
    void on_page3BackButton_clicked();

    void addShadow(QWidget *widget);

    void on_page4Button_clicked();

    void on_page4BackButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
