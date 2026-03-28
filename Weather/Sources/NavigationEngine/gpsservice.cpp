#include "gpsservice.h"

GpsService::GpsService(QObject *parent) : QObject(parent) {
    m_source = QGeoPositionInfoSource::createDefaultSource(this);
    if (m_source) {
        m_source->setUpdateInterval(1000); // каждую секунду
        connect(m_source, &QGeoPositionInfoSource::positionUpdated,
                this, &GpsService::onPositionUpdated);
    }
}

void GpsService::start() { if (m_source) m_source->startUpdates(); }
void GpsService::stop()  { if (m_source) m_source->stopUpdates(); }

void GpsService::onPositionUpdated(const QGeoPositionInfo &info) {
    m_lastPos = info.coordinate();
    double speed   = info.attribute(QGeoPositionInfo::GroundSpeed);
    double heading = info.attribute(QGeoPositionInfo::Direction);
    emit positionUpdated(m_lastPos, speed, heading);
}
