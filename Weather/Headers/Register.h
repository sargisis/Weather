#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QLabel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QMessageBox>

#include "../Headers/Interfaces/IRegisterWindow.h"
#include <memory>

class RegisterWindow : public QWidget, public IRegisterWindow
{
    Q_OBJECT
public:
    explicit RegisterWindow(QWidget *parent = nullptr);

private:
    std::unique_ptr<QLineEdit> email_input;
    std::unique_ptr<QLineEdit> password_input;
    std::unique_ptr<QLineEdit> confirm_input;
    std::unique_ptr<QPushButton> registerBtn;
    std::unique_ptr<QNetworkAccessManager> manager;

private:
    void RegisterUser(const QString& email , const QString& password) override;
signals:
    void registrationSuccess(const QString& email , const QString& passoword);
};
