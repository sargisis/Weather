#include "CenterLayout.h"

CenterLayout::CenterLayout(QWidget* parent)
    : QVBoxLayout(parent), m_networkManager(new QNetworkAccessManager(this))
{
    try {
        m_center_layout = new QVBoxLayout;
        m_center_label = new QLabel("Weather Info");
        m_center_label->setStyleSheet("background-color: yellow; color: blue");
        m_center_layout->addWidget(m_center_label);

        fetchWeatherData("40.3075", "44.0725");

        connect(m_networkManager, &QNetworkAccessManager::finished, this, &CenterLayout::onWeatherDataReceived);
    }catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void CenterLayout::createLayouts()
{
    addLayout(m_center_layout);
}

void CenterLayout::fetchWeatherData(const QString& latitude, const QString& longitude)
{
    try {
        QString apiKey = "716aa27058963431d3f2aafaeff2e033";
        QString urlStr = QString("https://api.openweathermap.org/data/2.5/weather?lat=%1&lon=%2&appid=%3&units=metric")
                             .arg(latitude).arg(longitude).arg(apiKey);

        QUrl url(urlStr);
        m_networkManager->get(QNetworkRequest(url));
    }catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

}

void CenterLayout::onWeatherDataReceived(QNetworkReply* reply)
{
    try {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObj = jsonDoc.object();

            double temperature = jsonObj.value(QString("main")).toObject().value(QString("temp")).toDouble(); // Get temperature
            QString weatherDescription = jsonObj.value(QString("weather")).toArray()[0].toObject().value(QString("description")).toString(); // Get weather description

            m_center_label->setText(QString("Temperature: %1 °C\nDescription: %2").arg(temperature).arg(weatherDescription));
        } else {
            m_center_label->setText("Error retrieving weather data: " + reply->errorString());
        }
        reply->deleteLater();
    }catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

