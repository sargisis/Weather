#include "routetracker.h"
#include <cmath>

RouteTracker::RouteTracker(QObject *parent) : QObject(parent) {}

void RouteTracker::setRoute(const Route &route) {
    m_route     = route;
    m_stepIndex = 0;
}

void RouteTracker::updatePosition(const QGeoCoordinate &pos) {
    if (!m_route.isValid()) return;

    // Проверяем отклонение от маршрута
    double dist = distanceToRoute(pos);
    if (dist > REROUTE_THRESHOLD) {
        emit offRoute(pos);
        return;
    }

    // Проверяем, прошли ли текущий шаг
    if (m_stepIndex < m_route.steps.size()) {
        auto &step = m_route.steps[m_stepIndex];
        double distToStep = pos.distanceTo(step.coordinate);

        if (distToStep < STEP_ADVANCE_DIST) {
            m_stepIndex++;
            if (m_stepIndex >= m_route.steps.size()) {
                emit arrived();
            } else {
                emit stepChanged(m_route.steps[m_stepIndex], m_stepIndex);
            }
        }
    }
}

double RouteTracker::distanceToRoute(const QGeoCoordinate &pos) const {
    double minDist = std::numeric_limits<double>::max();
    const auto &geom = m_route.geometry;

    // Ищем минимальное расстояние до отрезка маршрута
    for (int i = 0; i + 1 < geom.size(); ++i) {
        // Упрощённо: расстояние до узловых точек
        double d = pos.distanceTo(geom[i]);
        if (d < minDist) minDist = d;
    }
    return minDist;
}
