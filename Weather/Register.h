#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <memory>

class RegisterWindow : public QWidget
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

private slots:
    void onRegisterClicked();
};
