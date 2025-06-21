#include "Auth.h"
#include "Register.h"
#include "ForgotPassowrd.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QLabel>

AuthWindow::AuthWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(350, 340);
    this->setWindowTitle("Weather Login");
    this->setStyleSheet("background-color: #2b2b2b; color: white;");

    // === Style definitions ===
    QString inputStyle = "QLineEdit { padding: 8px; border-radius: 6px; background: #3c3f41; "
                         "color: white; border: 1px solid #555; }";

    QString btnStyle =
        "QPushButton { background-color: #0078D7; color: white; border: none; "
        "border-radius: 6px; padding: 8px 0; font-weight: bold; }"
        "QPushButton:hover { background-color: #005fa3; }";

    // === Widgets ===
    auto* title = new QLabel("Welcome Back 👋");
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);

    auto* email_input = new QLineEdit;
    email_input->setPlaceholderText("Email");
    email_input->setStyleSheet(inputStyle);

    auto* password_input = new QLineEdit;
    password_input->setPlaceholderText("Password");
    password_input->setEchoMode(QLineEdit::Password);
    password_input->setStyleSheet(inputStyle);

    auto* loginBtn = new QPushButton("Log In");
    auto* registerBtn = new QPushButton("Register");
    auto* forgotBtn = new QPushButton("Forgot Password");

    loginBtn->setStyleSheet(btnStyle);
    registerBtn->setStyleSheet(btnStyle);
    forgotBtn->setStyleSheet(btnStyle);

    // === Layout ===
    auto* layout = new QVBoxLayout;
    layout->addWidget(title);
    layout->addSpacing(10);
    layout->addWidget(email_input);
    layout->addWidget(password_input);
    layout->addSpacing(15);
    layout->addWidget(loginBtn);
    layout->addSpacing(5);
    layout->addWidget(registerBtn);
    layout->addSpacing(5);
    layout->addWidget(forgotBtn);
    layout->setContentsMargins(30, 20, 30, 20);
    setLayout(layout);

    // === Login ===
    connect(loginBtn, &QPushButton::clicked, this, [=]() {
        QJsonObject obj;
        obj["email"] = email_input->text();
        obj["password"] = password_input->text();

        QNetworkRequest req(QUrl("http://localhost:3000/login"));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        auto* manager = new QNetworkAccessManager(this);
        auto* reply = manager->post(req, QJsonDocument(obj).toJson());

        connect(reply, &QNetworkReply::finished, this, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                emit authSuccess();
                this->close();
            } else {
                QMessageBox::warning(this, "Login Failed", reply->readAll());
            }
            reply->deleteLater();
        });
    });

    // === Register ===
    connect(registerBtn, &QPushButton::clicked, this, [=]() {
        auto* regWin = new RegisterWindow();
        regWin->setAttribute(Qt::WA_DeleteOnClose);
        regWin->show();
    });

    // === Forgot Password ===
    connect(forgotBtn, &QPushButton::clicked, this, [=]() {
        auto* forgotWin = new ForgotPasswordWindow();
        forgotWin->setAttribute(Qt::WA_DeleteOnClose);
        forgotWin->show();
    });
}
