#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QUrl>
#include <QScreen>
#include <QGuiApplication>
#include <QLabel>
#include <QStyle>

#include "../Headers/Interfaces/IForgotPassword.h"

class ForgotPasswordWindow : public QWidget , public IForgotPassword
{
    Q_OBJECT
public:
    explicit ForgotPasswordWindow(QWidget *parent = nullptr);

private slots:
    void forgotPasswordUser(const QString& email) override;
    void onSendClicked();

private:
    QLineEdit* email_input;
    QPushButton* sendBtn;
    QNetworkAccessManager* manager;
};
