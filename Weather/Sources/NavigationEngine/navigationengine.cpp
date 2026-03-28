#include "navigationengine.h"

NavigationEngine::NavigationEngine(QObject *parent) : QObject(parent) {
    m_gps     = new GpsService(this);
    m_router  = new RoutingService(this);
    m_tracker = new RouteTracker(this);
    m_voice   = new VoiceService(this);

    // GPS → tracker
    connect(m_gps, &GpsService::positionUpdated,
            this, [this](QGeoCoordinate pos, double, double) {
        m_tracker->updatePosition(pos);
        emit positionChanged(pos);
    });

    // Новый маршрут → tracker + карта
    connect(m_router, &RoutingService::routeReady,
            this, [this](Route route) {
        m_tracker->setRoute(route);
        if (!route.steps.isEmpty()) {
            m_voice->announce(route.steps.first());
        }
        emit routeUpdated(route);
    });

    // Смена шага → голос
    connect(m_tracker, &RouteTracker::stepChanged,
            this, [this](RouteStep step, int) {
        m_voice->announce(step);
        emit currentStepChanged(step);
    });

    // Отклонение → перестроить
    connect(m_tracker, &RouteTracker::offRoute,
            this, [this](QGeoCoordinate pos) {
        RouteStep temp;
        temp.instruction = "rerouting";
        m_voice->announce(temp); 
        m_router->requestRoute(pos, m_destination);
    });

    // Прибытие
    connect(m_tracker, &RouteTracker::arrived,
            m_voice, &VoiceService::announceArrival);
}

void NavigationEngine::startNavigation(const QGeoCoordinate &destination) {
    m_destination = destination;
    m_gps->start();
    m_router->requestRoute(m_gps->lastPosition(), destination);
}

void NavigationEngine::stopNavigation() {
    m_gps->stop();
}
