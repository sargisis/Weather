#include "Register.h"
#include <QLabel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QMessageBox>

RegisterWindow::RegisterWindow(QWidget *parent)
    : QWidget(parent), manager(std::make_unique<QNetworkAccessManager>(this))
{
    setWindowTitle("Create an Account");
    setFixedSize(300, 160);
    setStyleSheet("background-color: #2b2b2b; color: white;");

    auto* title = new QLabel("Enter your Email");
    title->setStyleSheet("font-size: 18px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);

    email_input = std::make_unique<QLineEdit>();
    email_input->setPlaceholderText("Email");
    email_input->setStyleSheet("padding: 8px; background: #3c3f41; color: white;");

    registerBtn = std::make_unique<QPushButton>("Register");
    registerBtn->setStyleSheet(
        "QPushButton { background-color: #0078D7; color: white; padding: 8px; border-radius: 6px; }"
        "QPushButton:hover { background-color: #005fa3; }"
        );

    auto* layout = new QVBoxLayout;
    layout->addWidget(title);
    layout->addWidget(email_input.get());
    layout->addWidget(registerBtn.get());
    layout->setContentsMargins(20, 20, 20, 20);
    setLayout(layout);

    connect(registerBtn.get(), &QPushButton::clicked, this, &RegisterWindow::onRegisterClicked);
}


void RegisterWindow::onRegisterClicked()
{
    QString email = email_input->text().trimmed();
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter your email.");
        return;
    }

    QJsonObject obj;
    obj["email"] = email;

    QNetworkRequest req(QUrl("http://localhost:3000/register"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = manager->post(req, QJsonDocument(obj).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "Success", "Check your email to activate your account.");
            this->close();
        } else {
            QMessageBox::critical(this, "Registration Failed", reply->readAll());
        }
        reply->deleteLater();
    });
}

