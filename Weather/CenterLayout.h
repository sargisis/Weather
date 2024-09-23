#ifndef CENTERLAYOUT_H
#define CENTERLAYOUT_H

#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include "Layouts.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QJsonArray>
#include <exception>
#include <iostream>

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

    void fetchWeatherData(const QString& latitude, const QString& longitude);
};

#endif // CENTERLAYOUT_H
