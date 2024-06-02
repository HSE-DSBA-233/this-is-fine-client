#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <QDebug>
#include <QListWidget>
#include <QIcon>
#include <QSize>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QString>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QEasingCurve>
#include "chatsitemwidget.h"
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
        "QLineEdit:focus { border: 1px solid black; }"
        );
   
    ui->tokenLine->setStyleSheet("background-color: white; color: black; font-family: 'Montserrat'");
    ui->promptWidget->setStyleSheet("background-color: white; color: black; font-family: 'Montserrat'");
    logger->info("Applied application-wide stylesheet");

    // Add shadows to buttons
    QList<QPushButton*> buttons = {
        ui->page2Button,
        ui->homeButton,
        ui->chatButton,
        ui->settingsButton,
        ui->page4Button,
    };
    foreach (QPushButton* button, buttons) {
      
        addShadow(button, 20, 3);
        // Log shadow addition
        logger->info("Added shadow to button {}", button->objectName().toStdString());

        // Add press and release animations to buttons
        connect(button, &QPushButton::pressed, this, &MainWindow::animateButtonPress);
        connect(button, &QPushButton::released, this, &MainWindow::animateButtonRelease);
        logger->info("Connected animations to button {}", button->objectName().toStdString());
    }

    // Add shadows to widgets
    addShadow(ui->welcomeWidget, 30, 3);
    addShadow(ui->chatsWidget, 30, 3);
    addShadow(ui->chatWidget, 30, 3);
    addShadow(ui->chatInput2Widget, 20, 3);
    addShadow(ui->titleLabel, 30, 3);
    addShadow(ui->chatsTitleLabel, 30, 3);
    addShadow(ui->chatTitleLabel, 30, 3);

    // connect(ui->page2Button, &QPushButton::clicked, this, [=]() { animatePageTransition(1); });
    // connect(ui->page3BackButton, &QPushButton::clicked, this, [=]() { animatePageTransition(0); });
    // connect(ui->page3Button, &QPushButton::clicked, this, [=]() { animatePageTransition(2); });
    // connect(ui->homeButton, &QPushButton::clicked, this, [=]() { animatePageTransition(0); });
    // connect(ui->chatButton, &QPushButton::clicked, this, [=]() { animatePageTransition(3); });
    // connect(ui->settingsButton, &QPushButton::clicked, this, [=]() { animatePageTransition(4); });
    // connect(ui->page4Button, &QPushButton::clicked, this, [=]() { animatePageTransition(5); });

    connect(chatCreateWindow, &ChatCreateWindow::createChat, this, &MainWindow::handleCreateChat);

    ui->chatslistWidget->setStyleSheet(R"(
        QListWidget {
            background-color: white;
            border: 1px solid lightgray;
            padding: 15px;
            border-radius: 15px;
        }
        QListWidget::item {
            background-color: white;
            border: 1px solid lightgray;
            padding: 10px;
            border-radius: 5px;
        }
        QListWidget::item:selected {
            background-color: lightgray;
            color: black;
            border: 1px solid lightgray;
        }
        QListWidget::item:hover {
            background-color: lightgray;
        }
    )");
      
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

// Pages animations
// void MainWindow::animatePageTransition(int newIndex) {
//     QWidget* currentWidget = ui->stackedWidget->currentWidget();
//     QWidget* nextWidget = ui->stackedWidget->widget(newIndex);

//     int duration = 1000; // Duration of the animation in milliseconds
//     QEasingCurve curve = QEasingCurve::InOutQuad;

//     // Set the new index
//     ui->stackedWidget->setCurrentIndex(newIndex);

//     // Slide out the current widget
//     QPropertyAnimation* slideOut = new QPropertyAnimation(currentWidget, "pos");
//     slideOut->setDuration(duration);
//     slideOut->setStartValue(currentWidget->pos());
//     slideOut->setEndValue(QPoint(-currentWidget->width(), currentWidget->pos().y()));
//     slideOut->setEasingCurve(curve);

//     // Slide in the next widget
//     nextWidget->move(nextWidget->width(), nextWidget->pos().y());
//     QPropertyAnimation* slideIn = new QPropertyAnimation(nextWidget, "pos");
//     slideIn->setDuration(duration);
//     slideIn->setStartValue(QPoint(nextWidget->width(), nextWidget->pos().y()));
//     slideIn->setEndValue(QPoint(0, nextWidget->pos().y()));
//     slideIn->setEasingCurve(curve);

//     // Create an animation group to run both animations in parallel
//     QParallelAnimationGroup* group = new QParallelAnimationGroup;
//     group->addAnimation(slideOut);
//     group->addAnimation(slideIn);

//     // Start the animation
//     group->start(QAbstractAnimation::DeleteWhenStopped);
// }

void MainWindow::addShadow(QWidget *widget, int blur, int offset) {
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(blur);
    shadow->setOffset(0, offset);
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
            effect->setBlurRadius(20);
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
            effect->setOffset(0, 3);
            effect->setBlurRadius(20);
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
    chatCreateWindow->show();
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

void MainWindow::on_homeButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    getLogger()->info("Navigated back to home");
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
    getLogger()->info("Navigated to Rag Chat page");
}

void MainWindow::on_BackButtonToRAG_clicked()
{
    ui->stackedWidget->setCurrentIndex(8);
    getLogger()->info("Navigated back to Rag Model page");
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

void MainWindow::on_chatButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// Add chat
void MainWindow::handleCreateChat(const QString &title, const QString &datetime, const QString &model)
{
    QString itemText = QString("%1 | %2 | %3").arg(title, datetime, model);
    QListWidgetItem *item = new QListWidgetItem(ui->chatslistWidget);
    ChatsItemWidget *chatsItemWidget = new ChatsItemWidget(itemText);

    // connect(chatsItemWidget, &ChatsItemWidget::editClicked, this, &MainWindow::chatEditClicked);
    connect(chatsItemWidget, &ChatsItemWidget::deleteClicked, this, &MainWindow::chatDeleteClicked);

    item->setSizeHint(chatsItemWidget->sizeHint());
    ui->chatslistWidget->setItemWidget(item, chatsItemWidget);
}

// Enter chat
void MainWindow::on_chatslistWidget_itemClicked(QListWidgetItem *item)
{
    qDebug() << item->text();
    // QString itemTitle = item->text();
    // QStringList title = itemTitle.split(" | ");
    // ui->stackedWidget->setCurrentIndex(2);
    // ui->chatTitleLabel->setText(title[0]);
    // ui->chatModelLabel->setText(title[2]);
}

// Edit chat
// void MainWindow::chatEditClicked() {
//     ChatsItemWidget *chatsItemWidget = qobject_cast<ChatsItemWidget *>(sender());
//     QListWidgetItem *item = ui->chatslistWidget->itemAt(chatsItemWidget->pos());

// }

// Delete chat
void MainWindow::chatDeleteClicked() {
    ChatsItemWidget *chatsItemWidget = qobject_cast<ChatsItemWidget *>(sender());
    QListWidgetItem *item = ui->chatslistWidget->itemAt(chatsItemWidget->pos());
    if (QMessageBox::question(this, tr("Deleting a chat"),  tr("Are you sure you want to delete this chat?")) == QMessageBox::Yes) {
        delete item;
        delete chatsItemWidget;
    }
}

void MainWindow::saveMessageHistory()
{
    // Log saving message history
    getLogger()->info("Saved message history");
}
