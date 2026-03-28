#pragma once
#include <QObject>
#include "route.h"

class RouteTracker : public QObject {
    Q_OBJECT
public:
    explicit RouteTracker(QObject *parent = nullptr);
    void setRoute(const Route &route);
    void updatePosition(const QGeoCoordinate &pos);

    double distanceToRoute(const QGeoCoordinate &pos) const;
    int    currentStepIndex() const { return m_stepIndex; }

signals:
    void stepChanged(RouteStep step, int index);
    void offRoute(QGeoCoordinate currentPos);     // нужно перестроить
    void arrived();

private:
    Route  m_route;
    int    m_stepIndex = 0;
    static constexpr double REROUTE_THRESHOLD = 50.0;  // метры
    static constexpr double STEP_ADVANCE_DIST = 30.0;  // метры
};
