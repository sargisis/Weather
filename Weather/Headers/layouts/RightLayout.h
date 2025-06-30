#pragma once

#include <QVBoxLayout>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <memory>
#include <vector>
#include <QFrame>
#include "Layouts.h"

class RightLayout : public QVBoxLayout, public Layouts
{
public:
    explicit RightLayout(QWidget* parent = nullptr);
    void createLayouts() override;
    void fetchFutureWeather(const QString& city);

private:
    std::unique_ptr<QNetworkAccessManager> networkManager;
    std::vector<QFrame*> forecastCards;

    void onForecastData(QNetworkReply* reply);
    void clearCards();
    QFrame* createForecastCard(const QString& time, double temp, const QString& desc);
};
