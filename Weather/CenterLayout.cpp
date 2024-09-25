#include "CenterLayout.h"

CenterLayout::CenterLayout(QWidget* parent)
    : QVBoxLayout{parent},
    m_networkManager{new QNetworkAccessManager(this)},
    m_timer{new QTimer(this)}
{

        m_center_layout = new QVBoxLayout;

        m_center_label = new QLabel("Weather Info");
        m_center_label->setStyleSheet("background-color: yellow; color: blue");

        m_center_layout->addWidget(m_center_label);

        connect(m_networkManager, &QNetworkAccessManager::finished, this, &CenterLayout::onWeatherDataReceived);
}

void CenterLayout::createLayouts()
{
    addLayout(m_center_layout);
}

void CenterLayout::fetchWeatherData(const QString& latitude, const QString& longitude)
{

        QString apiKey = "716aa27058963431d3f2aafaeff2e033";
        QString urlStr = QString("https://api.openweathermap.org/data/2.5/weather?lat=%1&lon=%2&appid=%3&units=metric&nocache=%4")
                         .arg(latitude).arg(longitude).arg(apiKey).arg(QDateTime::currentSecsSinceEpoch());

        QUrl url(urlStr);
        m_networkManager->get(QNetworkRequest(url));
}

void CenterLayout::onWeatherDataReceived(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        double temperature = jsonObj.value(QString("main")).toObject().value(QString("temp")).toDouble();
        QString weatherDescription = jsonObj.value(QString("weather")).toArray()[0].toObject().value(QString("description")).toString();

        QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
        m_center_label->setText(QString("Temperature: %1 °C\n%2\nUpdated at: %3")
                                    .arg(temperature)
                                    .arg(weatherDescription)
                                    .arg(currentTime));
    } else {
        m_center_label->setText("Error retrieving weather data: " + reply->errorString());
    }
    reply->deleteLater();
}

void CenterLayout::fetchWeatherDataForCity(const QString& city)
{
    QString apiKey = "716aa27058963431d3f2aafaeff2e033";
    QString urlStr = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric&nocache=%3")
                         .arg(city).arg(apiKey).arg(QDateTime::currentSecsSinceEpoch());

    QUrl url(urlStr);
    m_networkManager->get(QNetworkRequest(url));
}

