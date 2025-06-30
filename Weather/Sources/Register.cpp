#include "../Headers/Register.h"

RegisterWindow::RegisterWindow(QWidget *parent)
    : QWidget(parent), manager(std::make_unique<QNetworkAccessManager>(this))
{
    setWindowTitle("Create an Account");
    setFixedSize(360, 300);
    setStyleSheet("background-color: #1e1e1e; color: white;");

    QString inputStyle = R"(
        QLineEdit {
            padding: 10px;
            font-size: 14px;
            border-radius: 8px;
            background: #2d2d2d;
            color: white;
            border: 1px solid #555;
        }
        QLineEdit:focus {
            border: 1px solid #0078D7;
        }
    )";

    QString btnStyle = R"(
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
    )";

    auto* title = new QLabel("Create Account");
    title->setStyleSheet("font-size: 22px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);

    email_input = std::make_unique<QLineEdit>();
    email_input->setPlaceholderText("Email");
    email_input->setStyleSheet(inputStyle);

    password_input = std::make_unique<QLineEdit>();
    password_input->setPlaceholderText("Password");
    password_input->setEchoMode(QLineEdit::Password);
    password_input->setStyleSheet(inputStyle);

    confirm_input = std::make_unique<QLineEdit>();
    confirm_input->setPlaceholderText("Confirm Password");
    confirm_input->setEchoMode(QLineEdit::Password);
    confirm_input->setStyleSheet(inputStyle);

    registerBtn = std::make_unique<QPushButton>("Register");
    registerBtn->setStyleSheet(btnStyle);
    registerBtn->setMinimumHeight(40);

    auto* layout = new QVBoxLayout;
    layout->addWidget(title);
    layout->addSpacing(10);
    layout->addWidget(email_input.get());
    layout->addWidget(password_input.get());
    layout->addWidget(confirm_input.get());
    layout->addSpacing(15);
    layout->addWidget(registerBtn.get());
    layout->addStretch();
    layout->setContentsMargins(30, 20, 30, 20);
    setLayout(layout);

    connect(registerBtn.get(), &QPushButton::clicked, this, [this]() {
        QString email = email_input->text().trimmed();
        QString password = password_input->text();
        QString confirm = confirm_input->text();

        if (email.isEmpty() || password.isEmpty() || confirm.isEmpty()) {
            QMessageBox::warning(this, "Error", "All fields are required.");
            return;
        }

        if (password != confirm) {
            QMessageBox::warning(this, "Error", "Passwords do not match.");
            return;
        }

        RegisterUser(email, password);
    });
}

void RegisterWindow::RegisterUser(const QString &email, const QString &password) {
    const QString API_KEY = "AIzaSyC6Jtos6Qydb0SuwCo8AFUSwyNJ-H7Vkys";

    QUrl url("https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + API_KEY);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader , "application/json");

    QJsonObject json;
    json["email"] = email;
    json["password"] = password;
    json["returnSecureToken"] = true;

    if (!manager){
        manager = std::make_unique<QNetworkAccessManager>(this);
    }
    connect(manager.get() , &QNetworkAccessManager::finished , this , [this, email, password](QNetworkReply* reply){
        if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray array = reply->readAll();
            qDebug() << "Registration Successful" << array;

            QMessageBox::information(this, "Success", "Account created successfully!");

            emit registrationSuccess(email , password);
            this->close();
        }
        else {
            qDebug() << "Registration Failed" << reply->errorString();
        }
        reply->deleteLater();
    });

    manager->post(request , QJsonDocument(json).toJson());
}
