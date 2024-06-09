#include "chatcreatewindow.h"
#include "ui_chatcreatewindow.h"
#include <QString>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

ChatCreateWindow::ChatCreateWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatCreateWindow)
{
    ui->setupUi(this);

    setWindowTitle("Creating a chat");

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::white);
    setPalette(palette);

    QList<QWidget*> elements = {
        ui->titleIconLabel,
        ui->titleLabel, ui->titleSelectWidget,
        ui->promptIconLabel,
        ui->promptTitleLabel,
        ui->promptSelectWidget,
        ui->modelIconLabel,
        ui->modelTitleLabel,
        ui->modelSelectWidget,
        ui->cancelButton,
        ui->createButton,
        ui->ragButton,
        ui->ragFileWidget,
        ui->ragIconLabel,
        ui->ragTitleLabel,
    };
    foreach (QWidget* element, elements) {
        addShadow(element, 20, 3);
    }

    QString comboBoxStyle = R"(
        QComboBox {
            background-color: white;
            padding: 5px;
            border: 1px solid lightgray;
            border-radius: 10px;
            color: black;
        }
        QComboBox:on {
            padding-top: 3px;
            padding-left: 4px;
        }
        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 30px;
            border-left: 1px solid lightgray;
        }
        QComboBox::down-arrow {
            image: url(:/assets/down-arrow.png);
            width: 12px;
            height: 12px;
        }
        QComboBox::down-arrow:on {
            top: 1px;
            left: 1px;
        }
        QComboBox QAbstractItemView {
            background-color: white;
            border: 1px solid lightgray;
        }
        QComboBox::item {
            selection-color: #FCCD4A;
            color: black;
        }
    )";

    ui->modelSelectWidget->setStyleSheet(comboBoxStyle);

}

ChatCreateWindow::~ChatCreateWindow()
{
    delete ui;
}

void ChatCreateWindow::addShadow(QWidget *widget, int blur, int offset) {
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(blur);
    shadow->setOffset(0, offset);
    shadow->setColor(Qt::gray);
    widget->setGraphicsEffect(shadow);
}

void ChatCreateWindow::on_createButton_clicked()
{
    QString title = ui->titleSelectWidget->text();
    QString prompt  = ui->promptSelectWidget->toPlainText();
    QString model = ui->modelSelectWidget->currentText();

    if (title.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please enter a title."));
        return;
    }

    emit createChat(title, prompt, model);

    accept();

    ui->titleSelectWidget->clear();
    ui->promptSelectWidget->setText("You are a helpful assistant.");
    ui->modelSelectWidget->setCurrentIndex(0);
}

void ChatCreateWindow::on_cancelButton_clicked()
{
    reject();
    ui->titleSelectWidget->clear();
    ui->ragFileWidget->clear();
}

void ChatCreateWindow::on_ragButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt);;All Files (*)"));
    QString fileContent;

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            fileContent = in.readAll();
            QFileInfo fileName(filePath);
            QString baseName = fileName.fileName();
            ui->ragFileWidget->setPlainText(baseName);
        }
    }

    // Here is the text from the file:
    qDebug() << fileContent;
}

