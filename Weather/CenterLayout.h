#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QJsonArray>

#include "Layouts.h"

class CenterLayout : public QVBoxLayout, public Layouts
{
    Q_OBJECT
public:
    explicit CenterLayout(QWidget* parent = nullptr);
    virtual void createLayouts() override;

private slots:
    void onWeatherDataReceived(QNetworkReply* reply);

private:
    QVBoxLayout* m_center_layout;
    QLabel* m_center_label;
    QNetworkAccessManager* m_networkManager;

public:
    void fetchWeatherData(const QString& latitude, const QString& longitude);
    void fetchWeatherDataForCity(const QString& city);
};
