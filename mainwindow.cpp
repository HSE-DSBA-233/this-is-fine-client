#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <QDebug>
#include <QListWidget>
#include <QIcon>
#include <QMessageBox>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Customizing
    setWindowTitle("This Is Fine");
    setWindowIcon(QIcon(":/assets/thisisfinelogo.png"));

    // Apply white background color to the main window
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::white);
    setPalette(palette);

    // Apply a style sheet to the entire application
    qApp->setStyleSheet(
        "QLabel {color: black; font-family: 'Montserrat'; }"
        "QPushButton { background-color: #FCCD4A; font-family: 'Montserrat'; color: white; border: 2px solid #FCCD4A; border-radius: 10px; padding: 5px 15px; }"
        "QPushButton:hover { background-color: #FFD971; }"
        "QListWidget { color: black; font-family: 'Montserrat'; border: none; }"
        );

    // Add shadows to buttons
    QList<QPushButton*> buttons = {ui->page1Button, ui->page2Button, ui->page2BackButton, ui->page3BackButton, ui->page3Button, ui->page4Button, ui->page4BackButton};
    foreach (QPushButton* button, buttons) {
        addShadow(button);

        // Add press and release animations to buttons
        connect(button, &QPushButton::pressed, this, &MainWindow::animateButtonPress);
        connect(button, &QPushButton::released, this, &MainWindow::animateButtonRelease);
    }

    // Add shadows to widgets
    addShadow(ui->widget_page1);
    addShadow(ui->projectsWidget);
    addShadow(ui->modelsWidget);
    addShadow(ui->promptWidget);

    // Add some data to projects widget
    QStringList items = {"Project 1", "Project 2", "Project 3"};
    ui->projectsWidget->addItems(items);
}

MainWindow::~MainWindow()
{
    saveMessageHistory();
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

void MainWindow::on_page1Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_page2Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_page2BackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_page3Button_clicked()
{
    QString selectedModel = ui->modelsWidget->currentText();
    QString selectedPrompt = ui->promptWidget->toPlainText();
    if (selectedModel.isEmpty() or selectedPrompt.isEmpty()) {
        QMessageBox::warning(this, "Inpxut Error", "Please select a model and type a prompt before chatting.");
        return;
    }
    ui->stackedWidget->setCurrentIndex(3);
    ui->modelLabel->setText(selectedModel);
    ui->promptLabel->setText(selectedPrompt);
}

void MainWindow::on_page3BackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_page4Button_clicked()
{
    QString message = ui->chatInput2Widget->text();
    if (!message.isEmpty()) {
        addMessage(true, message);
        addMessage(false, message);
        ui->chatInput2Widget->clear();
    }
}

void MainWindow::on_page4BackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::addMessage(bool isUser, const QString &message) {
    QString sender = isUser ? "You" : "Bot";

    ui->chatWindowWidget->append(QString("<div style='margin: 10px; padding: 15px;'><b> %1</b><br>%2</div>")
                                                                      .arg(sender, message));
}

void MainWindow::loadMessageHistory() {

}

void MainWindow::saveMessageHistory() {

}
