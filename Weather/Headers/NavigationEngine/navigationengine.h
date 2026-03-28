#pragma once
#include <QObject>
#include "gpsservice.h"
#include "routingservice.h"
#include "routetracker.h"
#include "voiceservice.h"

class NavigationEngine : public QObject {
    Q_OBJECT
public:
    explicit NavigationEngine(QObject *parent = nullptr);
    void startNavigation(const QGeoCoordinate &destination);
    void stopNavigation();

signals:
    void routeUpdated(Route route);          // отправить на карту
    void currentStepChanged(RouteStep step);
    void positionChanged(QGeoCoordinate pos);

private:
    GpsService     *m_gps;
    RoutingService *m_router;
    RouteTracker   *m_tracker;
    VoiceService   *m_voice;
    QGeoCoordinate  m_destination;
};
