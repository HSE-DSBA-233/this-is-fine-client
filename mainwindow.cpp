#include "mainwindow.h"
#include "chatsitemwidget.h"
#include "logger_util.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QEasingCurve>
#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>
#include <QIcon>
#include <QListWidget>
#include <QMessageBox>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QSize>
#include <QString>
#include <chat-api.h>
#include <json.hpp>
#include "chatclient.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include "chatsettingswindow.h"

using json = nlohmann::json;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), chatSettingsWindow(new chatsettingswindow(this)) {
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
      "QLineEdit:focus { border: 1px solid black; }"
      "QTextEdit {color: black; font-family: 'Montserrat'; }"
      "QPushButton { background-color: #FCCD4A; font-family: 'Montserrat'; "
      "color: white; border: 2px solid #FCCD4A; border-radius: 10px; padding: "
      "5px 15px; }"
      "QPushButton:hover { background-color: #FFD971; }"
      "QListWidget { color: black; font-family: 'Montserrat'; border: none; }"
      );
  logger->info("Applied application-wide stylesheet");

  // Add shadows to buttons
  QList<QPushButton *> buttons = {
      ui->createChatButton,
      ui->homeButton,
      ui->chatButton,
      ui->settingsButton,
      ui->sendMessageButton,
      ui->clearChatButton,
      ui->settingsChatButton,
      ui->chatRagButton,
  };
  foreach (QPushButton *button, buttons) {
    addShadow(button, 20, 3);

    // Add press and release animations to buttons
    connect(button, &QPushButton::pressed, this,
            &MainWindow::animateButtonPress);
    connect(button, &QPushButton::released, this,
            &MainWindow::animateButtonRelease);
    logger->info("Connected animations to button {}",
                 button->objectName().toStdString());
  }

  // Add shadows to widgets
  addShadow(ui->welcomeWidget, 30, 3);
  addShadow(ui->chatsWidget, 30, 3);
  addShadow(ui->chatWidget, 30, 3);
  addShadow(ui->chatInput2Widget, 20, 3);
  addShadow(ui->titleLabel, 30, 3);
  addShadow(ui->chatsTitleLabel, 30, 3);
  addShadow(ui->chatTitleLabel, 30, 3);
  addShadow(ui->settingsTokenWidget, 30, 3);
  addShadow(ui->settingsTitleLabel, 30, 3);
  logger->info("Added shadows to main widgets");
  
  connect(chatCreateWindow, &ChatCreateWindow::createChat, this,
          &MainWindow::handleCreateChat);
  logger->info("Connected chat creation signal");
  connect(chatSettingsWindow, &chatsettingswindow::updateChat, this,
          &MainWindow::handleUpdateChat);
  logger->info("Connected chat updating signal");
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
  logger->info("Set style sheet for chatslistWidget");

  ui->chatWindowWidget->clear();
  ui->chatslistWidget->clear();

    if (std::filesystem::exists("contexts") && std::filesystem::is_directory("contexts")) {
        for (const auto& entry : std::filesystem::directory_iterator("contexts")) {
            if (entry.path().extension() == ".json") {
                std::ifstream file(entry.path());
                if (file.is_open()) {
                    json context;
                    file >> context;
                    file.close();

                    std::string model = context["model"].get<std::string>();
                    std::string title = entry.path().stem().string();
                    QString itemText = QString("%1 | %2").arg(QString::fromStdString(title), QString::fromStdString(model));
                    QListWidgetItem *item = new QListWidgetItem(ui->chatslistWidget);
                    ChatsItemWidget *chatsItemWidget = new ChatsItemWidget(itemText);
                    connect(chatsItemWidget, &ChatsItemWidget::deleteClicked, this, &MainWindow::chatDeleteClicked);
                    item->setSizeHint(chatsItemWidget->sizeHint());
                    ui->chatslistWidget->setItemWidget(item, chatsItemWidget);
                    logger->info("Loaded chat: {}", itemText.toStdString());
                } else {
                    logger->warn("Could not open file: {}", entry.path().string());
                }
            }
        }
    }

  ui->stackedWidget->setCurrentIndex(1);

}

MainWindow::~MainWindow() {
  auto logger = getLogger();
  logger->info("MainWindow destroyed and message history saved");
  delete ui;
  delete chatSettingsWindow;
}

// Add shadow to elements
void MainWindow::addShadow(QWidget *widget, int blur, int offset) {
  auto logger = getLogger();
  QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
  shadow->setBlurRadius(blur);
  shadow->setOffset(0, offset);
  shadow->setColor(Qt::gray);
  widget->setGraphicsEffect(shadow);
  logger->info("Added shadow effect to widget {}",
               widget->objectName().toStdString());
}

// Animate buttons
void MainWindow::animateButtonPress() {
  auto logger = getLogger();
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    QGraphicsDropShadowEffect *effect =
        qobject_cast<QGraphicsDropShadowEffect *>(button->graphicsEffect());
    if (effect) {
      effect->setOffset(0, 0);
      effect->setBlurRadius(20);
    }
    logger->info("Animated button press for {}",
                 button->objectName().toStdString());
  }
}

void MainWindow::animateButtonRelease() {
  auto logger = getLogger();
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    QGraphicsDropShadowEffect *effect =
        qobject_cast<QGraphicsDropShadowEffect *>(button->graphicsEffect());
    if (effect) {
      effect->setOffset(0, 3);
      effect->setBlurRadius(20);
    }
    logger->info("Animated button release for {}",
                 button->objectName().toStdString());
  }
}
// Settings page button
void MainWindow::on_settingsButton_clicked()
{
    auto logger = getLogger();
    QString qtitle = ui->chatTitleLabel->text();
    ui->stackedWidget->setCurrentIndex(3);
    try {
        if (chatclient.end_chat(qtitle.toStdString())) {
            logger->info("Chat history saved successfully.");
        }
    } catch (const std::runtime_error &e) {
        // Skip
    } catch (const std::exception &e) {
        logger->warn("Error during end_chat: {}", e.what());
    }
    ui->chatWindowWidget->clear();
    ui->chatslistWidget->clear();
}


// Home page button
void MainWindow::on_homeButton_clicked() {
    auto logger = getLogger();
    QString qtitle = ui->chatTitleLabel->text();
    ui->stackedWidget->setCurrentIndex(0);
    logger->info("Navigated to Home page");
    try {
        if (chatclient.end_chat(qtitle.toStdString())) {
            logger->info("Chat history saved successfully.");
        }
    } catch (const std::runtime_error &e) {
        // Skip
    } catch (const std::exception &e) {
        logger->warn("Error during end_chat: {}", e.what());
    }
    ui->chatWindowWidget->clear();
    ui->chatslistWidget->clear();
}

// Chat page button
void MainWindow::on_chatButton_clicked() {
  auto logger = getLogger();
  QString qtitle = ui->chatTitleLabel->text();
  ui->stackedWidget->setCurrentIndex(1);
  logger->info("Navigated to Chat page");
    try {
        if (chatclient.end_chat(qtitle.toStdString())) {
            logger->info("Chat history saved successfully.");
        }
    } catch (const std::runtime_error &e) {
        // Skip
    } catch (const std::exception &e) {
        logger->warn("Error during end_chat: {}", e.what());
    }
    ui->chatWindowWidget->clear();
    ui->chatslistWidget->clear();

    if (std::filesystem::exists("contexts") && std::filesystem::is_directory("contexts")) {
        for (const auto& entry : std::filesystem::directory_iterator("contexts")) {
            if (entry.path().extension() == ".json") {
                std::ifstream file(entry.path());
                if (file.is_open()) {
                    json context;
                    file >> context;
                    file.close();

                    std::string model = context["model"].get<std::string>();
                    std::string title = entry.path().stem().string();
                    QString itemText = QString("%1 | %2").arg(QString::fromStdString(title), QString::fromStdString(model));
                    QListWidgetItem *item = new QListWidgetItem(ui->chatslistWidget);
                    ChatsItemWidget *chatsItemWidget = new ChatsItemWidget(itemText);
                    connect(chatsItemWidget, &ChatsItemWidget::deleteClicked, this, &MainWindow::chatDeleteClicked);
                    item->setSizeHint(chatsItemWidget->sizeHint());
                    ui->chatslistWidget->setItemWidget(item, chatsItemWidget);
                    logger->info("Loaded chat: {}", itemText.toStdString());
                } else {
                    logger->warn("Could not open file: {}", entry.path().string());
                }
            }
        }
    }
}

// Add chat
void MainWindow::handleCreateChat(const QString &title, const QString &prompt,
                                  const QString &model, const QString &rag) {
  auto logger = getLogger();
  QString itemText = QString("%1 | %2").arg(title, model);
  QListWidgetItem *item = new QListWidgetItem(ui->chatslistWidget);
  ChatsItemWidget *chatsItemWidget = new ChatsItemWidget(itemText);
  // connect(chatsItemWidget, &ChatsItemWidget::editClicked, this,
  // &MainWindow::chatEditClicked);
  connect(chatsItemWidget, &ChatsItemWidget::deleteClicked, this, &MainWindow::chatDeleteClicked);

  item->setSizeHint(chatsItemWidget->sizeHint());
  ui->chatslistWidget->setItemWidget(item, chatsItemWidget);
  logger->info("Created new chat: {}", itemText.toStdString());

  json promptJson = {
      {{"role", "system"}, {"content", prompt.toStdString()}}
  };

  json dialogue = json::array();

  chatclient.start_chat(model.toStdString(), promptJson, dialogue);

  if(!rag.isEmpty()) {
      chatclient.add_rag(rag.toStdString());
      ui->chatRagButton->setText("RAG");
  } else {
      ui->chatRagButton->setText("NO RAG LOCKED");
  }

  json chat_json = {{"model", model.toStdString()}, {"prompt", prompt.toStdString()}, {"title", title.toStdString()}, {"dialogue", dialogue}, {"rag", rag.toStdString()}};
  
    std::string dir_path = "contexts/";
    std::string file_path = dir_path + title.toStdString() + ".json";

    // Ensure the directory exists
    std::filesystem::create_directories(dir_path);

    // Open and write to the file
    std::ofstream out(file_path);
    out << chat_json.dump(4);
    out.close();
    std::cout << "Chat history saved to " << file_path << '\n';
}

// Update chat
void MainWindow::handleUpdateChat(const QString &title, const QString &prompt,
                                  const QString &model, const QString &rag) {
    auto logger = getLogger();
    QString titleOld = ui->chatTitleLabel->text();

    std::string oldFilePathJson = "contexts/" + titleOld.toStdString() + ".json";
    std::string newFilePathJson = "contexts/" + title.toStdString() + ".json";

    std::string oldFilePathZip = "contexts/" + titleOld.toStdString() + ".zip";
    std::string newFilePathZip = "contexts/" + title.toStdString() + ".zip";

    std::ifstream inputFile(oldFilePathJson);

    if (!inputFile) {
        logger->error("Could not open the file for reading: {}", oldFilePathJson);
        QMessageBox::warning(this, tr("Warning"), tr("Could not open the file for reading: ") + QString::fromStdString(oldFilePathJson));
        return;
    }

    json jsonChat;
    inputFile >> jsonChat;
    inputFile.close();

    jsonChat["model"] = model.toStdString();
    jsonChat["prompt"] = prompt.toStdString();
    jsonChat["title"] = title.toStdString();
    jsonChat["rag"] = rag.toStdString();

    std::ofstream outputFile(oldFilePathJson);
    if (!outputFile) {
        logger->error("Could not open the file for writing: {}", oldFilePathJson);
        QMessageBox::warning(this, tr("Warning"), tr("Could not open the file for writing: ") + QString::fromStdString(oldFilePathJson));
        return;
    }
    outputFile << jsonChat.dump(4);
    outputFile.close();

    json promptJson = {
        {{"role", "system"}, {"content", prompt.toStdString()}}
    };

    if (chatclient.change_model(model.toStdString())) {
        logger->info("Model was changed");
    } else {
        logger->warn("Error with a model");
    }

    if (chatclient.change_prompt(promptJson)) {
        logger->info("Prompt was changed");
    } else {
        logger->warn("Error with a prompt change");
    }

    if (chatclient.add_rag(rag.toStdString())) {
        ui->chatRagButton->setText("RAG");
        logger->info("RAG was changed");
    } else {
        logger->warn("Error with a RAG");
    }

    if (title.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please enter a title."));
        return;
    }

    if (prompt.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please enter a prompt."));
        return;
    }

    if (rag.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please pin a file for RAG."));
        return;
    }

    ui->chatTitleLabel->setText(title);
    ui->chatModelLabel->setText(model);

    // Rename the JSON file to the new title
    if (std::rename(oldFilePathJson.c_str(), newFilePathJson.c_str()) != 0) {
        logger->error("Error renaming file from {} to {}", oldFilePathJson, newFilePathJson);
        QMessageBox::warning(this, tr("Warning"), tr("Error renaming file."));
    } else {
        logger->info("File renamed successfully from {} to {}", oldFilePathJson, newFilePathJson);
    }

    // Rename the ZIP file to the new title
    if (std::rename(oldFilePathZip.c_str(), newFilePathZip.c_str()) != 0) {
        logger->error("Error renaming file from {} to {}", oldFilePathZip, newFilePathZip);
        // QMessageBox::warning(this, tr("Warning"), tr("Error renaming file."));
    } else {
        logger->info("File renamed successfully from {} to {}", oldFilePathZip, newFilePathZip);
    }

    ui->stackedWidget->setCurrentIndex(2);
}

// Enter chat
void MainWindow::on_chatslistWidget_itemClicked(QListWidgetItem *item) {
    auto logger = getLogger();
    ChatsItemWidget *chatsItemWidget = qobject_cast<ChatsItemWidget *>(ui->chatslistWidget->itemWidget(item));

    QString itemTitle = chatsItemWidget->getTitle();
    QStringList title = itemTitle.split(" | ");
    ui->stackedWidget->setCurrentIndex(2);
    ui->chatTitleLabel->setText(title[0]);
    ui->chatModelLabel->setText(title[1]);

    logger->info("Entered chat: {}", itemTitle.toStdString());

    std::ifstream ifs(fmt::format("contexts/{}.json", title[0].toStdString()));
    json context;
    ifs >> context;
    ifs.close();

    if (!context.is_null()) {
        QString rag = QString::fromStdString(context["rag"]);
        if (rag == "") {
            ui->chatRagButton->setText("NO RAG LOCKED");
        } else {
            ui->chatRagButton->setText("RAG");
        }
        for (const auto &message : context["dialogue"]) {
            QString content = QString::fromStdString(message["content"]);
            QString role = QString::fromStdString(message["role"]);
            bool isUser = (role == "user");
            addMessage(isUser, content);
            logger->info("Added message from {}: {}", role.toStdString(), content.toStdString());
        }
    }

    chatclient.load_chat(fmt::format("contexts/{}.zip", title[0].toStdString()));
    
}

// Delete chat
void MainWindow::chatDeleteClicked() {
    auto logger = getLogger();
    ChatsItemWidget *chatsItemWidget = qobject_cast<ChatsItemWidget *>(sender());
    QListWidgetItem *item = ui->chatslistWidget->itemAt(chatsItemWidget->pos());
    QString qtitle = chatsItemWidget->getTitle().split(" | ").first();
    std::string title = qtitle.toStdString();
    if (QMessageBox::question(this, tr("Deleting a chat"), tr("Are you sure you want to delete this chat?")) == QMessageBox::Yes) {
        logger->info("Deleted chat: {}", title);
        delete item;
        delete chatsItemWidget;
        std::string filePathJson = "contexts/" + title + ".json";
        std::string filePathZip = "contexts/" + title + ".zip";
        std::filesystem::remove(filePathJson);
        std::filesystem::remove(filePathZip);
    } else {
        logger->info("Cancelled deletion of chat: {}", title);
    }
}

// Chat create button
void MainWindow::on_createChatButton_clicked() {
    auto logger = getLogger();
    chatCreateWindow->show();
    logger->info("Chat create window shown");    
}

// Chat send button
void MainWindow::on_sendMessageButton_clicked() {
    auto logger = getLogger();
    QString message = ui->chatInput2Widget->text();
    if (!message.isEmpty()) {
            addMessage(true, message);

            QString rag_status = ui->chatRagButton->text();
            std::string response;
            if (rag_status.toStdString() == "NO RAG" or rag_status.toStdString() == "NO RAG LOCKED") {
                response = chatclient.generate_response(message.toStdString());
            } else if (rag_status.toStdString() == "RAG") {
                response = chatclient.rag_generate(message.toStdString());
            } else {
                logger->warn("Rag status error");
            }

            std::cout << "Generated response: " << response << '\n';
            addMessage(false, QString::fromStdString(response));
            ui->chatInput2Widget->clear();
            logger->info("Message added: {}", message.toStdString());

            QString title = ui->chatTitleLabel->text();
            std::string file_path = "contexts/" + title.toStdString() + ".json";

            // Load existing chat JSON
            std::ifstream ifs(file_path);
            json chat_json;
            ifs >> chat_json;
            ifs.close();

            // Update dialogue
            json user_message = {{"role", "user"}, {"content", message.toStdString()}};
            json bot_response = {{"role", "assistant"}, {"content", response}};
            chat_json["dialogue"].push_back(user_message);
            chat_json["dialogue"].push_back(bot_response);

            // Save updated chat JSON
            std::ofstream ofs(file_path);
            ofs << chat_json.dump(4);
            ofs.close();
    } else {
        logger->warn("Attempted to add an empty message");
    }
}

// Rag change
void MainWindow::on_chatRagButton_clicked()
{
    std::string rag_status =  ui->chatRagButton->text().toStdString();
    if (rag_status == "RAG") {
        ui->chatRagButton->setText("NO RAG");
    } else if (rag_status == "NO RAG") {
        ui->chatRagButton->setText("RAG");
    }
}


// Chat add message
void MainWindow::addMessage(bool isUser, const QString &message) {
    auto logger = getLogger();
    QString sender = isUser ? "You" : "Bot";

    ui->chatWindowWidget->append(
        QString(
            "<div style='margin: 10px; padding: 15px;'><b> %1</b><br>%2</div>")
            .arg(sender, message));
    logger->info("Added message from {}: {}", sender.toStdString(),
                 message.toStdString());
}


// Clear chat
void MainWindow::on_clearChatButton_clicked(){

    auto logger = getLogger();
    if(chatclient.clear_context()) {
        logger->info("The context was cleared");
        ui->chatWindowWidget->setText("");
              QString title = ui->chatTitleLabel->text();
        std::string file_path = "contexts/" + title.toStdString() + ".json";

        // Load existing chat JSON
        std::ifstream ifs(file_path);
        if (ifs.is_open()) {
            json chat_json;
            ifs >> chat_json;
            ifs.close();

            // Clear dialogue
            chat_json["dialogue"] = json::array();

            // Save updated chat JSON
            std::ofstream ofs(file_path);
            ofs << chat_json.dump(4); // Save with pretty-print
            ofs.close();

            logger->info("Cleared dialogue in JSON file: {}", file_path);
        } else {
            logger->warn("Could not open file for clearing dialogue: {}", file_path);
        }
    }
    else {
        logger->warn("Error during clear_context");
    }
}

// Settings in Chat page
void MainWindow::on_settingsChatButton_clicked()
{
    auto logger = getLogger();
    QString title = ui->chatTitleLabel->text();

    std::string filePathJson = "contexts/" + title.toStdString() + ".json";
    std::ifstream inputFile(filePathJson);

    json jsonChat;
    inputFile >> jsonChat;
    inputFile.close();

    QString prompt = QString::fromStdString(jsonChat["prompt"]);
    QString model = QString::fromStdString(jsonChat["model"]);
    QString rag = QString::fromStdString(jsonChat["rag"]);

    emit passChatSettings(title, prompt, model, rag);

    chatSettingsWindow->show();

    logger->info("Chat settings window shown");
}
