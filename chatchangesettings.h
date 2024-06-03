#ifndef CHATCHANGESETTINGS_H
#define CHATCHANGESETTINGS_H

#include <QDialog>

namespace Ui {
class ChatChangeSettings;
}

class ChatChangeSettings : public QDialog
{
    Q_OBJECT

public:
    explicit ChatChangeSettings(QWidget *parent = nullptr);
    ~ChatChangeSettings();

signals:
    void createChat(const QString &title, const QString &prompt, const QString &model);

private slots:
    void addShadow(QWidget *widget, int blur, int offset);

    void on_createButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ChatChangeSettings *ui;
};

#endif // CHATCHANGESETTINGS_H
