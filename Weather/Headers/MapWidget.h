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

// Класс MapWidget реализует собственный движок загрузки тайлов карты.
class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget* parent = nullptr);
    ~MapWidget() override;

    // Установка центра карты
    void setCenter(double lat, double lon, int zoom = 12);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    double m_centerLat;
    double m_centerLon;
    int m_zoom;
    
    QPoint m_lastMousePos;
    bool m_isDragging = false;

    QNetworkAccessManager* m_networkManager;
    QMap<QString, QPixmap> m_tileCache; // Кэш: "z_x_y" -> QPixmap
    QMap<QString, QNetworkReply*> m_pendingRequests;

    // Географические функции (Web Mercator Projection)
    QPointF latLonToWorldPoint(double lat, double lon, int zoom) const;
    void worldPointToLatLon(QPointF worldPoint, int zoom, double& lat, double& lon) const;
    void updateVisibleTiles();
    void downloadTile(int z, int x, int y);
    QString tileKey(int z, int x, int y) const;
};
