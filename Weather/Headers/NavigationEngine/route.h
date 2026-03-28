#pragma once
#include <QList>
#include <QString>
#include <QGeoCoordinate>

struct RouteStep {
    QString       instruction;   // "turn", "arrive", "depart"...
    QString       streetName;
    double        distance = 0;  // метры
    double        duration = 0;  // секунды
    QGeoCoordinate coordinate;
};

struct Route {
    QList<QGeoCoordinate> geometry;  // полилиния маршрута
    QList<RouteStep>      steps;
    double totalDistance = 0;
    double totalDuration = 0;
    bool   isValid() const { return !geometry.isEmpty(); }
};
