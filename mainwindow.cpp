#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QFontDatabase>
#include <QDebug>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QListWidget>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Customizing
    this->setWindowTitle("This Is Fine");
    this->setWindowIcon(QIcon("/Users/pavelpopov/Downloads/thisisfinelogo.png"));

    // Apply white background color to the main window
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::white);
    this->setPalette(palette);

    // Apply a style sheet to the entire application
    qApp->setStyleSheet(
        "QLabel { color: black; font-family: 'Montserrat'; }"
        "QPushButton { background-color: #FCCD4A; font-family: 'Montserrat'; color: white; border: 2px solid #FCCD4A; border-radius: 10px; padding: 5px 15px; }"
        "QPushButton:hover { background-color: #FFD971; }"
        "QListWidget { color: black; font-family: 'Montserrat'; border: none; }"
        );

    // Add shadows to buttons
    QList<QPushButton*> buttons = {ui->btn_page1, ui->btn_page2, ui->btn_page1_back, ui->btn_page2_back};
    foreach (QPushButton* button, buttons) {
        addShadow(button);

        // Add press and release animations to buttons
        connect(button, &QPushButton::pressed, this, &MainWindow::animateButtonPress);
        connect(button, &QPushButton::released, this, &MainWindow::animateButtonRelease);
    }

    // Add shadows to windows
    addShadow(ui->text);
    addShadow(ui->projects);
    // addShadow(ui->models);
    addShadow(ui->prompt);

    // Add some data to projects window
    QStringList items = {"Project 1", "Project 2", "Project 3"};
    ui->projects->addItems(items);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addShadow(QWidget *widget) {
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(50);
    shadow->setOffset(0, 5);
    shadow->setColor(Qt::gray);
    widget->setGraphicsEffect(shadow);
}

void MainWindow::animateButtonPress()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QGraphicsDropShadowEffect *effect = qobject_cast<QGraphicsDropShadowEffect*>(button->graphicsEffect());
        if (effect) {
            effect->setOffset(0, 0);
            effect->setBlurRadius(50);
        }
    }
}

void MainWindow::animateButtonRelease()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QGraphicsDropShadowEffect *effect = qobject_cast<QGraphicsDropShadowEffect*>(button->graphicsEffect());
        if (effect) {
            effect->setOffset(0, 5);
            effect->setBlurRadius(50);
        }
    }
}

void MainWindow::on_btn_page1_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_btn_page1_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btn_page2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_btn_page2_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

