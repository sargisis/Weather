#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QGeoCoordinate>
#include "route.h"

class RoutingService : public QObject {
    Q_OBJECT
public:
    explicit RoutingService(QObject *parent = nullptr);
    void requestRoute(const QGeoCoordinate &from, const QGeoCoordinate &to);

signals:
    void routeReady(Route route);
    void routeError(QString message);

private slots:
    void onReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_nam;
    // Можно self-host OSRM или использовать router.project-osrm.org
    QString m_baseUrl = "https://router.project-osrm.org/route/v1/driving";
};
