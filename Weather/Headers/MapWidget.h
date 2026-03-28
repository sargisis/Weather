#pragma once

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QMap>
#include <QPoint>
#include <QPointF>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QPainterPath>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QCompleter>

#include <QGeoCoordinate>
#include "NavigationEngine/route.h"

// Класс MapWidget реализует собственный движок загрузки тайлов карты.
class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget* parent = nullptr);
    ~MapWidget() override;

    // Установка центра карты
    void setCenter(double lat, double lon, int zoom = 12);

    // Отрисовка маршрута из NavigationEngine
    void setRoute(const Route& route);

    // Обновление позиции GPS-маркера
    void updateGpsPosition(const QGeoCoordinate& pos);

public slots:
    void calculateRoute(const QString& fromCity, const QString& toCity);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private slots:
    void onRouteButtonClicked();

private:
    double m_centerLat;
    double m_centerLon;
    int m_zoom;
    
    QPoint m_lastMousePos;
    bool m_isDragging = false;

    // ----- UI элементы поверх карты -----
    class QLineEdit* m_fromInput;
    class QLineEdit* m_toInput;
    class QPushButton* m_routeBtn;

    // ----- Геометрия маршрута -----
    QList<QPointF> m_routePath; // Массив Lat, Lon
    QList<QPointF> decodePolyline(const QString& encoded) const;
    void requestOSRMRoute(double lat1, double lon1, double lat2, double lon2);

    QNetworkAccessManager* m_networkManager;
    QMap<QString, QPixmap> m_tileCache; // Кэш: "z_x_y" -> QPixmap
    QMap<QString, QNetworkReply*> m_pendingRequests;

    // ----- GPS маркер -----
    QGeoCoordinate m_gpsPos;
    bool m_hasGps = false;

    // ----- Метаданные маршрута для HUD -----
    double m_routeDistance = 0;  // метры
    double m_routeDuration = 0;  // секунды
    QPointF m_routeStart;        // lat, lon старта маршрута (A)
    QPointF m_routeEnd;          // lat, lon финиша маршрута (B)
    bool m_hasRoute = false;

    // ----- Кеш QPainterPath маршрута (для быстрой отрисовки) -----
    QPainterPath m_cachedRoutePath;
    int          m_cachedRouteZoom = -1;
    double       m_cachedRouteCenterX = 0;
    double       m_cachedRouteCenterY = 0;

    // ----- Таймер дебонсинга загрузки тайлов -----
    QTimer* m_tileLoadTimer = nullptr;

    // Географические функции (Web Mercator Projection)
    QPointF latLonToWorldPoint(double lat, double lon, int zoom) const;
    void worldPointToLatLon(QPointF worldPoint, int zoom, double& lat, double& lon) const;
    void updateVisibleTiles();
    void downloadTile(int z, int x, int y);
    QString tileKey(int z, int x, int y) const;
};
