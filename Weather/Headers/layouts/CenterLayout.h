#pragma once

#include <QVBoxLayout>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <memory>
#include <vector>
#include "Layouts.h"

class CenterLayout : public QVBoxLayout, public Layouts
{
    Q_OBJECT
public:
    explicit CenterLayout(QWidget* parent = nullptr);
    void createLayouts() override;
    void fetchWeatherDataForCity(const QString& city);

private slots:
    void onWeatherDataReceived(QNetworkReply* reply);

private:
    std::unique_ptr<QNetworkAccessManager> networkManager;
    std::vector<QLabel*> weatherLabels;
};
