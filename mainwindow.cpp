#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <QDebug>
#include <QListWidget>
#include <QIcon>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDir>
#include "logger_util.h" // Include the logger utility

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize logger
    auto logger = getLogger();
    spdlog::set_level(spdlog::level::debug);

    // Log MainWindow initialization
    logger->info("MainWindow initialized");

    // Customizing
    setWindowTitle("This Is Fine");
    setWindowIcon(QIcon(":/assets/thisisfinelogo.png"));
    logger->info("Window title and icon set");

    // Apply white background color to the main window
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::white);
    setPalette(palette);
    logger->info("Applied white background color to main window");

    // Apply a style sheet to the entire application
    qApp->setStyleSheet(
        "QLabel {color: black; font-family: 'Montserrat'; }"
        "QLineEdit {color: black; font-family: 'Montserrat'; }"
        "QTextEdit {color: black; font-family: 'Montserrat'; }"
        "QPushButton { background-color: #FCCD4A; font-family: 'Montserrat'; color: white; border: 2px solid #FCCD4A; border-radius: 10px; padding: 5px 15px; }"
        "QPushButton:hover { background-color: #FFD971; }"
        "QListWidget { color: black; font-family: 'Montserrat'; border: none; }"
        );
    ui->tokenLine->setStyleSheet("background-color: white; color: black; font-family: 'Montserrat'");
    ui->promptWidget->setStyleSheet("background-color: white; color: black; font-family: 'Montserrat'");
    logger->info("Applied application-wide stylesheet");

    // Add shadows to buttons
    QList<QPushButton*> buttons = {ui->page1Button, ui->page2Button, ui->page2BackButton, ui->page3BackButton, ui->page3Button, ui->page4Button, ui->page4BackButton};
    foreach (QPushButton* button, buttons) {
        addShadow(button);
        // Log shadow addition
        logger->info("Added shadow to button {}", button->objectName().toStdString());

        // Add press and release animations to buttons
        connect(button, &QPushButton::pressed, this, &MainWindow::animateButtonPress);
        connect(button, &QPushButton::released, this, &MainWindow::animateButtonRelease);
        logger->info("Connected animations to button {}", button->objectName().toStdString());
    }

    // Add shadows to widgets
    addShadow(ui->widget_page1);
    addShadow(ui->projectsWidget);
    addShadow(ui->modelsWidget);
    addShadow(ui->promptWidget);
    logger->info("Added shadows to main widgets");

    // Add some data to projects widget
    QStringList items = {"Project 1", "Project 2", "Project 3"};
    ui->projectsWidget->addItems(items);
    logger->info("Added items to projects widget");
}

MainWindow::~MainWindow()
{
    saveMessageHistory();
    delete ui;
    getLogger()->info("MainWindow destroyed and message history saved");
}

void MainWindow::addShadow(QWidget *widget) {
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(50);
    shadow->setOffset(0, 5);
    shadow->setColor(Qt::gray);
    widget->setGraphicsEffect(shadow);
    getLogger()->info("Added shadow to widget {}", widget->objectName().toStdString());
}

void MainWindow::animateButtonPress()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QGraphicsDropShadowEffect *effect = qobject_cast<QGraphicsDropShadowEffect*>(button->graphicsEffect());
        if (effect) {
            effect->setOffset(0, 0);
            effect->setBlurRadius(50);
            getLogger()->debug("Button {} pressed", button->objectName().toStdString());
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
            getLogger()->debug("Button {} released", button->objectName().toStdString());
        }
    }
}

void MainWindow::on_page1Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    getLogger()->info("Navigated to page 1");
}

void MainWindow::on_page1AboutButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    getLogger()->info("Navigated to about page");
}

void MainWindow::on_page1ContactButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    getLogger()->info("Navigated to contact page");
}

void MainWindow::on_page1SettingsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    getLogger()->info("Navigated to settings page");
}

void MainWindow::on_page2Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
    getLogger()->info("Navigated to page 8");
}

void MainWindow::on_baseModelButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    getLogger()->info("Navigated to page base model");
}

void MainWindow::on_ragModelButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    getLogger()->info("Navigated to page rag");
}


void MainWindow::on_page2BackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    getLogger()->info("Navigated back to main page");
}

void MainWindow::on_page3Button_clicked()
{
    QString selectedModel = ui->modelsWidget->currentText();
    QString selectedPrompt = ui->promptWidget->toPlainText();
    if (selectedModel.isEmpty() or selectedPrompt.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select a model and type a prompt before chatting.");
        getLogger()->warn("Input error: model or prompt not selected");
        return;
    }
    ui->stackedWidget->setCurrentIndex(3);
    ui->modelLabel->setText(selectedModel);
    ui->promptLabel->setText(selectedPrompt);
    getLogger()->info("Navigated to page 3 with model {} and prompt {}", selectedModel.toStdString(), selectedPrompt.toStdString());
}

//get token
void MainWindow::on_getTokenButton_clicked()
{
    QString token = ui->tokenLine->text();

    QFile file("/Users/mverzhbitskiy/Documents/GitHub/This-is-Fine-client/token.txt");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << token;
        file.close();
        QMessageBox::information(this, "Success", "Token was successfully saved");
        getLogger()->info("Token saved to token.txt");
    }
    else
    {
        QMessageBox::warning(this, "Error", "Could not open file for writing");
        getLogger()->error("Failed to open token.txt for writing");
    }
}

void MainWindow::on_page3BackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    getLogger()->info("Navigated back to page 1 from page 3");
}

void MainWindow::on_page4Button_clicked()
{
    QString message = ui->chatInput2Widget->text();
    if (!message.isEmpty()) {
        addMessage(true, message);
        addMessage(false, message);
        ui->chatInput2Widget->clear();
        getLogger()->info("User sent message: {}", message.toStdString());
    } else {
        getLogger()->warn("Message input was empty");
    }
}

void MainWindow::on_page4BackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    getLogger()->info("Navigated back to page 2 from page 4");
}

void MainWindow::addMessage(bool isUser, const QString &message) {
    QString sender = isUser ? "You" : "Bot";
    ui->chatWindowWidget->append(QString("<div style='margin: 10px; padding: 15px;'><b> %1</b><br>%2</div>").arg(sender, message));
    getLogger()->info("{} added message: {}", sender.toStdString(), message.toStdString());
}

void MainWindow::on_page5BackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    getLogger()->info("Navigated back to main page from page 5");
}

void MainWindow::on_RagChatButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
}

void MainWindow::on_BackButtonToRAG_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
}

void MainWindow::on_page6BackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    getLogger()->info("Navigated back to main page from page 6");
}

void MainWindow::on_page7BackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    getLogger()->info("Navigated back to main page from page 7");
}

void MainWindow::on_page9BackButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    getLogger()->info("Navigated back to page 2");
}

void MainWindow::loadMessageHistory()
{
    // Log loading message history
    getLogger()->info("Loaded message history");
}

void MainWindow::saveMessageHistory()
{
    // Log saving message history
    getLogger()->info("Saved message history");
}
