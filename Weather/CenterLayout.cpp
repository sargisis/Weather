#include "CenterLayout.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QFrame>
#include <QNetworkRequest>

CenterLayout::CenterLayout(QWidget* parent)
    : QVBoxLayout(parent), networkManager(std::make_unique<QNetworkAccessManager>(parent))
{
    createLayouts();
    connect(networkManager.get(), &QNetworkAccessManager::finished, this, &CenterLayout::onWeatherDataReceived);
}

void CenterLayout::createLayouts()
{
    auto* card = new QFrame;
    card->setObjectName("centerCard");

    auto* layout = new QVBoxLayout(card);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(10);

    card->setStyleSheet(R"(
        #centerCard {
            background-color: #1e1e1e;
            border-radius: 20px;
        }
        QLabel {
            color: #ffffff;
            font-size: 16px;
            padding: 5px;
        }
    )");

    QStringList labels = {
        "🌡️ Temperature: —",
        "☁️ Description: —",
        "📊 Pressure: —",
        "💧 Humidity: —",
        "🌬️ Wind Speed: —",
        "🕒 Updated at: —"
    };

    for (const auto& text : labels) {
        auto* lbl = new QLabel(text);
        lbl->setWordWrap(true);
        layout->addWidget(lbl);
        weatherLabels.push_back(lbl);
    }

    this->addWidget(card);
}

void CenterLayout::fetchWeatherDataForCity(const QString& city)
{
    QString apiKey = "716aa27058963431d3f2aafaeff2e033";
    QString url = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric")
                      .arg(city, apiKey);
    networkManager->get(QNetworkRequest(QUrl(url)));
}

void CenterLayout::onWeatherDataReceived(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        if (!weatherLabels.empty())
            weatherLabels[0]->setText("❌ Error: " + reply->errorString());
        reply->deleteLater();
        return;
    }

    auto json = QJsonDocument::fromJson(reply->readAll()).object();
    auto main = json.value("main").toObject();
    auto wind = json.value("wind").toObject();
    auto weatherArr = json.value("weather").toArray();

    if (weatherLabels.size() < 6 || main.isEmpty() || wind.isEmpty() || weatherArr.isEmpty()) {
        weatherLabels[0]->setText("⚠️ Incomplete weather data.");
        reply->deleteLater();
        return;
    }

    const auto& weather = weatherArr[0].toObject();

    weatherLabels[0]->setText(QString("🌡️ Temperature: %1 °C").arg(main["temp"].toDouble()));
    weatherLabels[1]->setText(QString("☁️ Description: %1").arg(weather["description"].toString()));
    weatherLabels[2]->setText(QString("📊 Pressure: %1 hPa").arg(main["pressure"].toInt()));
    weatherLabels[3]->setText(QString("💧 Humidity: %1%").arg(main["humidity"].toInt()));
    weatherLabels[4]->setText(QString("🌬️ Wind Speed: %1 m/s").arg(wind["speed"].toDouble()));
    weatherLabels[5]->setText(QString("🕒 Updated at: %1").arg(QDateTime::currentDateTime().toString("hh:mm:ss")));

    reply->deleteLater();
}
