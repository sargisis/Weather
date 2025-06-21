#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class ForgotPasswordWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ForgotPasswordWindow(QWidget *parent = nullptr);

private slots:
    void onSendClicked();

private:
    QLineEdit* email_input;
    QPushButton* sendBtn;
    QNetworkAccessManager* manager;
};
