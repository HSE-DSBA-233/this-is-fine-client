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
    QList<QPushButton*> buttons = {ui->btn_start, ui->btn_newproject, ui->btn_start_back};
    foreach (QPushButton* button, buttons) {
        QGraphicsDropShadowEffect *buttonShadow = new QGraphicsDropShadowEffect;
        buttonShadow->setBlurRadius(50);
        buttonShadow->setOffset(0, 5);
        buttonShadow->setColor(Qt::gray);
        button->setGraphicsEffect(buttonShadow);

        // // Add press and release animations
        connect(button, &QPushButton::pressed, this, &MainWindow::animateButtonPress);
        connect(button, &QPushButton::released, this, &MainWindow::animateButtonRelease);
    }

    // Add shadow to text widget
    QGraphicsDropShadowEffect *textShadow = new QGraphicsDropShadowEffect;
    textShadow->setBlurRadius(50);
    textShadow->setOffset(0, 5);
    textShadow->setColor(Qt::gray);
    ui->text->setGraphicsEffect(textShadow);

    // Add shadow to projects widget
    QGraphicsDropShadowEffect *projectsShadow = new QGraphicsDropShadowEffect;
    projectsShadow->setBlurRadius(50);
    projectsShadow->setOffset(0, 5);
    projectsShadow->setColor(Qt::gray);
    ui->projects->setGraphicsEffect(projectsShadow);

    // Add some data to projects
    QStringList items = {"Project 1", "Project 2", "Project 3"};
    ui->projects->addItems(items);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (projectsWindow)
        delete projectsWindow;
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

void MainWindow::on_btn_start_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_btn_start_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
