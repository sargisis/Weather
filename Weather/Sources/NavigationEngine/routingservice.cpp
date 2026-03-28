#include "routingservice.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

RoutingService::RoutingService(QObject *parent) : QObject(parent) {
    m_nam = new QNetworkAccessManager(this);
    connect(m_nam, &QNetworkAccessManager::finished,
            this, &RoutingService::onReply);
}

void RoutingService::requestRoute(const QGeoCoordinate &from, const QGeoCoordinate &to) {
    QString url = QString("%1/%2,%3;%4,%5?steps=true&annotations=true&overview=full&geometries=geojson")
        .arg(m_baseUrl)
        .arg(from.longitude()).arg(from.latitude())
        .arg(to.longitude()).arg(to.latitude());
    m_nam->get(QNetworkRequest(QUrl(url)));
}

void RoutingService::onReply(QNetworkReply *reply) {
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        emit routeError(reply->errorString());
        return;
    }

    auto doc = QJsonDocument::fromJson(reply->readAll());
    auto routesArray = doc.object()["routes"].toArray();
    if (routesArray.isEmpty()) return;

    auto routeObj = routesArray[0].toObject();
    auto legs = routeObj["legs"].toArray();
    
    Route route;
    route.totalDistance = routeObj["distance"].toDouble();
    route.totalDuration = routeObj["duration"].toDouble();

    // Парсим координаты геометрии
    auto coords = routeObj["geometry"].toObject()["coordinates"].toArray();
    for (const auto &c : coords) {
        auto pt = c.toArray();
        route.geometry.append(QGeoCoordinate(pt[1].toDouble(), pt[0].toDouble()));
    }

    // Парсим шаги (манёвры)
    for (const auto &legValue : legs) {
        auto leg = legValue.toObject();
        for (const auto &stepValue : leg["steps"].toArray()) {
            auto step = stepValue.toObject();
            auto maneuver = step["maneuver"].toObject();
            
            RouteStep s;
            s.instruction  = maneuver["type"].toString();
            s.streetName   = step["name"].toString();
            s.distance     = step["distance"].toDouble();
            s.duration     = step["duration"].toDouble();
            auto loc = maneuver["location"].toArray();
            s.coordinate   = QGeoCoordinate(loc[1].toDouble(), loc[0].toDouble());
            route.steps.append(s);
        }
    }
    emit routeReady(route);
}
