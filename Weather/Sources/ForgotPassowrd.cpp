#include "../Headers/ForgotPassowrd.h"

ForgotPasswordWindow::ForgotPasswordWindow(QWidget *parent)
    : QWidget(parent), manager(new QNetworkAccessManager(this))
{
    setWindowTitle("🔐 Reset Password");
    setFixedSize(380, 200);
    setStyleSheet(R"(
        QWidget {
            background-color: #1e1e1e;
            color: white;
            border-radius: 12px;
        }
        QLineEdit {
            padding: 10px;
            font-size: 14px;
            border-radius: 8px;
            background: #2c2c2c;
            color: white;
            border: 1px solid #555;
        }
        QPushButton {
            background-color: #0078D7;
            color: white;
            border-radius: 8px;
            padding: 10px;
            font-size: 14px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #005fa3;
        }
    )");

    auto* title = new QLabel("Enter your email to reset password");
    title->setStyleSheet("font-size: 16px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);

    email_input = new QLineEdit;
    email_input->setPlaceholderText("Your email address");

    sendBtn = new QPushButton("Send Reset Link");

    auto* layout = new QVBoxLayout;
    layout->addWidget(title);
    layout->addSpacing(10);
    layout->addWidget(email_input);
    layout->addSpacing(10);
    layout->addWidget(sendBtn);
    layout->addStretch();
    layout->setContentsMargins(20, 20, 20, 20);
    setLayout(layout);

    // Центрируем окно
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    connect(sendBtn, &QPushButton::clicked, this, &ForgotPasswordWindow::onSendClicked);
}

void ForgotPasswordWindow::onSendClicked() {
    QString email = email_input->text().trimmed();
    if (!email.isEmpty())
    {
        forgotPasswordUser(email);
    }
    else
    {
        QMessageBox::warning(this, "Input Error", "Please enter your email.");
    }
}

void ForgotPasswordWindow::forgotPasswordUser(const QString &email)
{
    const QString API_KEY = "AIzaSyC6Jtos6Qydb0SuwCo8AFUSwyNJ-H7Vkys";

   QUrl url("https://identitytoolkit.googleapis.com/v1/accounts:sendOobCode?key=" + API_KEY);

    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader , "application/json");

    QJsonObject json;
    json["requestType"] = "PASSWORD_RESET";
    json["email"] = email;

    QNetworkReply* reply = manager->post(request , QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QByteArray response = reply->readAll();
        qDebug() << "Firebase response:" << response;

        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "Success", "Reset email sent");
        } else {
            QMessageBox::warning(this, "Error", reply->errorString());
        }

        reply->deleteLater();
    });
}
