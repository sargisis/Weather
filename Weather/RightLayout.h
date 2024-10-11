#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDateTime>
#include <QString>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "Layouts.h"

class RightLayout : public QVBoxLayout, public Layouts
{
public:
    explicit RightLayout(QWidget* parent = nullptr);
    virtual void createLayouts() override;
    void fetchFutureWeather(const QString& city);

private:
    QVBoxLayout* m_right_layout {nullptr};
    QLabel* m_right_label {nullptr};
    QNetworkAccessManager* m_network_manager {nullptr};
    QString m_apiKey;

    void handleNetworkInfo(QNetworkReply* reply);
};
