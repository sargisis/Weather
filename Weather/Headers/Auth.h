#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QLabel>
#include <QDesktopServices>
#include <QSettings>
#include <QUrlQuery>
#include <QRegularExpression>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAction>

#include "../Headers/Register.h"
#include "../Headers/ForgotPassowrd.h"
#include "../Headers/Login.h"

class AuthWindow : public QWidget {
    Q_OBJECT
public:
    explicit AuthWindow(QWidget* parent = nullptr);

signals:
    void authSuccess();

private:
    void handleGoogleSignIn();
    void startLocalServer();
    void exchangeCodeForToken(const QString& code);

    QLineEdit* email_input;
    QLineEdit* password_input;
    bool passwordVisible = false;

    QTcpServer* server = nullptr;
    QNetworkAccessManager* network = nullptr;
};
