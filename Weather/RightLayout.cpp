#include "RightLayout.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QNetworkRequest>

RightLayout::RightLayout(QWidget* parent)
    : QVBoxLayout(parent), networkManager(std::make_unique<QNetworkAccessManager>(parent))
{
    createLayouts();
    connect(networkManager.get(), &QNetworkAccessManager::finished, this, &RightLayout::onForecastData);
}

void RightLayout::createLayouts()
{
    setSpacing(15);
    setContentsMargins(10, 10, 10, 10);
    setAlignment(Qt::AlignTop);
}

void RightLayout::fetchFutureWeather(const QString& city)
{
    clearCards();
    QString apiKey = "716aa27058963431d3f2aafaeff2e033";
    QString url = QString("https://api.openweathermap.org/data/2.5/forecast?q=%1&appid=%2&units=metric")
                      .arg(city, apiKey);
    networkManager->get(QNetworkRequest(QUrl(url)));
}

void RightLayout::onForecastData(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        auto* error = createForecastCard("Error", 0, "Failed to load forecast");
        this->addWidget(error);
        forecastCards.push_back(error);
        reply->deleteLater();
        return;
    }

    auto json = QJsonDocument::fromJson(reply->readAll()).object();
    auto list = json["list"].toArray();

    QSet<QString> uniqueDays;
    for (const auto& item : list) {
        auto entry = item.toObject();
        QString dt_txt = entry["dt_txt"].toString();
        QDateTime dateTime = QDateTime::fromString(dt_txt, "yyyy-MM-dd HH:mm:ss");

        // Show only 12:00 forecasts for uniqueness
        if (dateTime.time().hour() != 12)
            continue;

        QString dayKey = dateTime.date().toString("yyyy-MM-dd");
        if (uniqueDays.contains(dayKey))
            continue;

        uniqueDays.insert(dayKey);

        double temp = entry["main"].toObject()["temp"].toDouble();
        QString desc = entry["weather"].toArray()[0].toObject()["description"].toString();
        QString timeStr = dateTime.toString("ddd dd MMM");

        auto* card = createForecastCard(timeStr, temp, desc);
        this->addWidget(card);
        forecastCards.push_back(card);

        if (forecastCards.size() >= 5)
            break;
    }

    reply->deleteLater();
}

QFrame* RightLayout::createForecastCard(const QString& time, double temp, const QString& desc)
{
    auto* card = new QFrame;
    auto* layout = new QVBoxLayout(card);

    auto* label = new QLabel(
        QString("🗓️ <b>%1</b><br>🌡️ %2 °C<br>☁️ %3")
            .arg(time)
            .arg(temp)
            .arg(desc)
        );
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignLeft);

    card->setStyleSheet(R"(
        QFrame {
            background-color: #0a84ff;
            color: white;
            padding: 14px;
            border-radius: 14px;
            font-size: 14px;
        }
        QLabel {
            font-size: 14px;
        }
    )");

    layout->addWidget(label);
    return card;
}

void RightLayout::clearCards()
{
    for (auto* card : forecastCards) {
        this->removeWidget(card);
        delete card;
    }
    forecastCards.clear();
}
