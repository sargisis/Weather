#include "../Headers/Auth.h"

AuthWindow::AuthWindow(QWidget* parent)
    : QWidget(parent)
{
    this->setFixedSize(380, 460);
    this->setWindowTitle("Weather Login");
    this->setStyleSheet("background-color: #1e1e1e; color: white; border-radius: 12px;");

    // Auto-login if refresh_token exists
    QSettings settings("MyCompany", "WeatherApp");
    QString savedToken = settings.value("refresh_token").toString();
    if (!savedToken.isEmpty()) {
        emit authSuccess();
        this->close();
        return;
    }

    // Styles
    QString inputStyle = R"(
        QLineEdit {
            padding: 10px;
            font-size: 14px;
            border-radius: 8px;
            background: #2c2c2c;
            color: white;
            border: 1px solid #444;
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
            padding: 8px;
            font-weight: bold;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #005fa3;
        }
    )";

    QString googleBtnStyle = R"(
        QPushButton {
            background-color: #4285F4;
            color: white;
            border-radius: 8px;
            padding: 8px;
            font-weight: bold;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #3367D6;
        }
    )";

    // Title and subtitle
    auto* title = new QLabel("Welcome Back 👋");
    title->setStyleSheet("font-size: 24px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);

    auto* subtitle = new QLabel("Sign in to continue");
    subtitle->setStyleSheet("font-size: 14px; color: #aaaaaa;");
    subtitle->setAlignment(Qt::AlignCenter);

    // Inputs
    email_input = new QLineEdit;
    email_input->setPlaceholderText("Email");
    email_input->setStyleSheet(inputStyle);

    password_input = new QLineEdit;
    password_input->setPlaceholderText("Password");
    password_input->setEchoMode(QLineEdit::Password);
    password_input->setStyleSheet(inputStyle);

    // Buttons
    auto* loginBtn = new QPushButton("Log In");
    auto* registerBtn = new QPushButton("Register");
    auto* forgotBtn = new QPushButton("Forgot Password");
    auto* googleBtn = new QPushButton("Sign in with Google");

    loginBtn->setStyleSheet(btnStyle);
    registerBtn->setStyleSheet(btnStyle);
    forgotBtn->setStyleSheet(btnStyle);
    googleBtn->setStyleSheet(googleBtnStyle);

    int btnHeight = 35;
    loginBtn->setMinimumHeight(btnHeight);
    registerBtn->setMinimumHeight(btnHeight);
    forgotBtn->setMinimumHeight(btnHeight);
    googleBtn->setMinimumHeight(btnHeight);

    auto* divider = new QLabel("──────────  or  ──────────");
    divider->setAlignment(Qt::AlignCenter);
    divider->setStyleSheet("color: #777; font-size: 12px;");

    // Layout
    auto* layout = new QVBoxLayout;
    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addSpacing(10);
    layout->addWidget(email_input);
    layout->addWidget(password_input);
    layout->addSpacing(10);
    layout->addWidget(loginBtn);
    layout->addWidget(registerBtn);
    layout->addWidget(forgotBtn);
    layout->addSpacing(10);
    layout->addWidget(divider);
    layout->addSpacing(10);
    layout->addWidget(googleBtn);
    layout->addStretch();
    layout->setContentsMargins(30, 20, 30, 20);
    setLayout(layout);

    // Connections
    connect(loginBtn, &QPushButton::clicked, this, [this]() {
        QString email = email_input->text().trimmed();
        QString password = password_input->text();

        if (email.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please enter both email and password.");
            return;
        }

        auto* logInWin = new LogINWindow();
        logInWin->setAttribute(Qt::WA_DeleteOnClose);
        connect(logInWin, &QObject::destroyed, this, [this, &logInWin]() {
            if (logInWin->wasSuccessful()) {
                emit authSuccess();
                this->close();
            }
        });
        logInWin->LogInUser(email, password);
    });



    connect(registerBtn, &QPushButton::clicked, this, [this]() {
        auto* regWin = new RegisterWindow();
        regWin->setAttribute(Qt::WA_DeleteOnClose);

        connect(regWin, &RegisterWindow::registrationSuccess, this,
                [this](const QString& email, const QString& password) {
                    auto* logInWin = new LogINWindow();
                    logInWin->setAttribute(Qt::WA_DeleteOnClose);

                    connect(logInWin, &QObject::destroyed, this, [this, &logInWin]() {
                        if (logInWin->wasSuccessful()) {
                            emit authSuccess();
                            this->close();
                        }
                    });

                    logInWin->LogInUser(email, password);
                });

        regWin->show();
    });


    connect(forgotBtn, &QPushButton::clicked, this, [=]() {
        auto* forgotWin = new ForgotPasswordWindow();
        forgotWin->setAttribute(Qt::WA_DeleteOnClose);
        forgotWin->show();
    });

    connect(googleBtn, &QPushButton::clicked, this, &AuthWindow::handleGoogleSignIn);

    server = new QTcpServer(this);
    network = new QNetworkAccessManager(this);
}

void AuthWindow::handleGoogleSignIn() {
    startLocalServer();

    const QString clientId = "61085252023-kgrp7cksj5vqs6bet9q3ldfaot81nraf.apps.googleusercontent.com";
    const QString redirectUri = "http://localhost:8080";

    QString authUrl = QString("https://accounts.google.com/o/oauth2/v2/auth?"
                              "client_id=%1&"
                              "redirect_uri=%2&"
                              "response_type=code&"
                              "scope=openid%20email%20profile&"
                              "access_type=offline").arg(clientId, redirectUri);

    QDesktopServices::openUrl(QUrl(authUrl));
}

void AuthWindow::startLocalServer() {
    connect(server, &QTcpServer::newConnection, this, [this]() {
        QTcpSocket* socket = server->nextPendingConnection();
        if (!socket) return;

        if (!socket->waitForReadyRead(2000)) return;

        QString request = socket->readAll();
        QRegularExpression re("GET /\\?code=([^&\\s]+)");
        QRegularExpressionMatch match = re.match(request);
        if (match.hasMatch()) {
            QString code = match.captured(1);
            exchangeCodeForToken(code);
            socket->write("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                          "<h2>You may close this window now.</h2>");
        } else {
            socket->write("HTTP/1.1 400 Bad Request\r\n\r\nInvalid redirect.");
        }
        socket->disconnectFromHost();
        server->close();
    });

    if (!server->listen(QHostAddress::LocalHost, 8080)) {
        QMessageBox::warning(this, "Server Error", "Cannot listen on localhost:8080");
    }
}

void AuthWindow::exchangeCodeForToken(const QString& code) {
    const QString clientId = "61085252023-kgrp7cksj5vqs6bet9q3ldfaot81nraf.apps.googleusercontent.com";
    const QString clientSecret = "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";
    const QString redirectUri = "http://localhost:8080";

    QUrl tokenUrl("https://oauth2.googleapis.com/token");
    QNetworkRequest request(tokenUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("code", code);
    params.addQueryItem("client_id", clientId);
    params.addQueryItem("client_secret", clientSecret);
    params.addQueryItem("redirect_uri", redirectUri);
    params.addQueryItem("grant_type", "authorization_code");

    connect(network, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray replyData = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(replyData);
            QString refreshToken = doc["refresh_token"].toString();

            QSettings settings("MyCompany", "WeatherApp");
            settings.setValue("refresh_token", refreshToken);

            QMessageBox::information(this, "Google Login", "Login successful!");
            emit authSuccess();
            this->close();
        } else {
            QMessageBox::warning(this, "Google Login Error", reply->errorString());
        }
        reply->deleteLater();
    });

    network->post(request, params.toString(QUrl::FullyEncoded).toUtf8());
}

