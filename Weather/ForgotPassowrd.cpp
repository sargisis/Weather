#include "ForgotPassowrd.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

ForgotPasswordWindow::ForgotPasswordWindow(QWidget *parent)
    : QWidget(parent), manager(new QNetworkAccessManager(this))
{
    setWindowTitle("Reset Password");
    setFixedSize(300, 150);

    email_input = new QLineEdit;
    email_input->setPlaceholderText("Enter your email");

    sendBtn = new QPushButton("Send Reset Link");

    auto* layout = new QVBoxLayout;
    layout->addWidget(email_input);
    layout->addWidget(sendBtn);
    setLayout(layout);

    connect(sendBtn, &QPushButton::clicked, this, &ForgotPasswordWindow::onSendClicked);
}

void ForgotPasswordWindow::onSendClicked()
{
    QString email = email_input->text().trimmed();
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your email.");
        return;
    }

    QJsonObject obj;
    obj["email"] = email;

    QNetworkRequest request(QUrl("http://localhost:3000/forgot"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = manager->post(request, QJsonDocument(obj).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "Sent", "Check your email to reset your password.");
            this->close();
        } else {
            QMessageBox::critical(this, "Error", reply->readAll());
        }
        reply->deleteLater();
    });
}
