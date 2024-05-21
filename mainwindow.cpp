#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QFontDatabase>
#include <QDebug>
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Apply white background color to the main window
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::white);
    this->setPalette(palette);

    // Load the Inter font
    int id = QFontDatabase::addApplicationFont(":/fonts/Inter-Regular.ttf");
    if (id == -1) {
        qWarning() << "Failed to load Inter font!";
    } else {
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont interFont(family);
        qDebug() << "Loaded Inter font:" << family;
    }

    // Apply a style sheet to the entire application
    qApp->setStyleSheet(
        "QLabel { color: black; }"  // Black text for QLabel
        "QPushButton { background-color: #fccb48; color: white; "
        "border: 2px solid #fccb48; border-radius: 10px; padding: 5px 15px; }"  // Yellow background, white text, and rounded corners for QPushButton
        "QListWidget { color: black; background-color: lightgray; "
        "font-family: 'Inter'; font-size: 16px; border: none; }"  // No border, black text, light gray background, Inter font, and larger font size for QListWidget
        "#backgroundFrame { background-color: #F1F1F1; border-radius: 15px; }"  // Rounded background and color for backgroundFrame
        "#BackMainButton, #NewProjectButton { background-color: #fccb48; color: white; "
        "border: 2px solid #fccb48; border-radius: 10px; padding: 5px 15px; }"  // Styling for BackMainButton and NewProjectButton
        );

    // Add shadows to buttons
    QList<QPushButton*> buttons = {ui->pushButton, ui->BackMainButton, ui->NewProjectButton};
    foreach (QPushButton* button, buttons) {
        QGraphicsDropShadowEffect *buttonShadow = new QGraphicsDropShadowEffect;
        buttonShadow->setBlurRadius(10);
        buttonShadow->setOffset(2, 2);
        buttonShadow->setColor(Qt::gray);
        button->setGraphicsEffect(buttonShadow);

        // Add press and release animations
        connect(button, &QPushButton::pressed, this, &MainWindow::animateButtonPress);
        connect(button, &QPushButton::released, this, &MainWindow::animateButtonRelease);
    }

    // Initially hide the list placeholder, BackMainButton, and NewProjectButton
    ui->listPlaceholder->setVisible(false);
    ui->BackMainButton->setVisible(false);
    ui->NewProjectButton->setVisible(false);

    // Connect the button signals to the slots
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    connect(ui->BackMainButton, &QPushButton::clicked, this, &MainWindow::on_BackMainButton_clicked);
    connect(ui->NewProjectButton, &QPushButton::clicked, this, &MainWindow::on_NewProjectButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::animateButtonPress()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QGraphicsDropShadowEffect *effect = qobject_cast<QGraphicsDropShadowEffect*>(button->graphicsEffect());
        if (effect) {
            effect->setOffset(-2, -2);
            effect->setBlurRadius(5);
        }
    }
}

void MainWindow::animateButtonRelease()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QGraphicsDropShadowEffect *effect = qobject_cast<QGraphicsDropShadowEffect*>(button->graphicsEffect());
        if (effect) {
            effect->setOffset(2, 2);
            effect->setBlurRadius(10);
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << "Button clicked, updating UI...";
    showProjectListScreen();
}

void MainWindow::on_BackMainButton_clicked()
{
    qDebug() << "Back button clicked, returning to main screen...";
    showMainScreen();
}

void MainWindow::on_NewProjectButton_clicked()
{
    qDebug() << "New project button clicked, no action defined yet...";
    // Define new project functionality here in the future
}

void MainWindow::showMainScreen()
{
    // Hide the list placeholder and show the main UI elements
    ui->listPlaceholder->setVisible(false);
    ui->pushButton->setVisible(true);
    ui->label->setVisible(true);
    ui->backgroundFrame->setVisible(true);
    ui->BackMainButton->setVisible(false);
    ui->NewProjectButton->setVisible(false);
}

void MainWindow::showProjectListScreen()
{
    // Clear the layout of the placeholder widget
    QLayout *currentLayout = ui->listPlaceholder->layout();
    if (currentLayout != nullptr) {
        QLayoutItem *item;
        while ((item = currentLayout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
        delete currentLayout;
    }

    // Create a new layout for the project list screen
    QVBoxLayout *layout = new QVBoxLayout;
    QListWidget *listWidget = new QListWidget(this);

    // Add projects to the list widget
    QStringList projects = getProjectList(); // Fetch the project list
    qDebug() << "Projects:" << projects;
    for (const QString &project : projects) {
        listWidget->addItem(project);
    }

    // Apply a style sheet to the list widget
    listWidget->setStyleSheet("QListWidget::item { padding: 5px; }");

    // Add the list widget to the layout
    layout->addWidget(listWidget);

    // Set the new layout to the placeholder widget
    ui->listPlaceholder->setLayout(layout);

    // Make the placeholder visible
    ui->listPlaceholder->setVisible(true);

    // Show the Back and New Project buttons
    ui->BackMainButton->setVisible(true);
    ui->NewProjectButton->setVisible(true);

    // Hide the main UI elements
    ui->pushButton->setVisible(false);
    ui->label->setVisible(false);
    ui->backgroundFrame->setVisible(true);  // Ensure backgroundFrame remains visible
}

QStringList MainWindow::getProjectList()
{
    // Example function to return a list of project names
    // Replace this with actual data fetching logic
    return QStringList{"Project 1", "Project 2", "Project 3"};
}
