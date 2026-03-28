#pragma once
#include <QObject>
#include <QGeoPositionInfoSource>
#include <QGeoCoordinate>

class GpsService : public QObject {
    Q_OBJECT
public:
    explicit GpsService(QObject *parent = nullptr);
    void start();
    void stop();
    QGeoCoordinate lastPosition() const { return m_lastPos; }

signals:
    void positionUpdated(QGeoCoordinate coord, double speedMps, double headingDeg);

private slots:
    void onPositionUpdated(const QGeoPositionInfo &info);

private:
    QGeoPositionInfoSource *m_source = nullptr;
    QGeoCoordinate m_lastPos;
};
