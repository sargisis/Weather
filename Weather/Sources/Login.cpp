#include "../Headers/Login.h"


LogINWindow::LogINWindow(QWidget* parent)
    : QWidget(parent)
    , manager (std::make_unique<QNetworkAccessManager>(this))
{
    this->setWindowTitle("Logging in...");
    this->resize(300, 150);

    auto* layout = new QVBoxLayout(this);
    auto* label = new QLabel("Please wait...");
    label->setStyleSheet("color: white; font-size: 14px;");
    layout->addWidget(label);
    this->setLayout(layout);

    this->setStyleSheet("background-color: #2b2b2b;");
}



void LogINWindow::LogInUser(const QString &email, const QString &password)
{
    const QString API_KEY = "AIzaSyC6Jtos6Qydb0SuwCo8AFUSwyNJ-H7Vkys";

    QUrl url("https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + API_KEY);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject object;
    object["email"] = email;
    object["password"] = password;
    object["returnSecureToken"] = true;

    reply = manager->post(request , QJsonDocument(object).toJson());
    QObject::connect(manager.get(), &QNetworkAccessManager::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray array = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(array);
            QString idToken = doc.object()["idToken"].toString();
            QString email = doc.object()["email"].toString();

            // Создаём и позиционируем сообщение по центру
            auto* msgBox = new QMessageBox(this);
            msgBox->setWindowTitle("LogIn Successful");
            msgBox->setText("Welcome: " + email);
            msgBox->setIcon(QMessageBox::Information);
            msgBox->setStyleSheet("QMessageBox { background-color: #2b2b2b; color: white; }");

            msgBox->adjustSize();
            QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
            int x = (screenGeometry.width() - msgBox->width()) / 2;
            int y = (screenGeometry.height() - msgBox->height()) / 2;
            msgBox->move(x, y);

            msgBox->exec();

            loginSuccessful = true;
            this->close();
        } else {
            QMessageBox::warning(this , "Error", "Please check your email or password:\n" + reply->errorString());
        }
        reply->deleteLater();
    });
}

