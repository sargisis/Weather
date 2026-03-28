#include "MapWidget.h"
#include <QPainter>
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <cmath>
#include <QMouseEvent>
#include <QWheelEvent>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

MapWidget::MapWidget(QWidget* parent)
    : QWidget(parent), m_centerLat(40.1811), m_centerLon(44.5136), m_zoom(11) // Default: Yerevan
{
    m_networkManager = new QNetworkAccessManager(this);
}

MapWidget::~MapWidget() {}

void MapWidget::setCenter(double lat, double lon, int zoom)
{
    m_centerLat = lat;
    m_centerLon = lon;
    m_zoom = zoom;
    updateVisibleTiles();
    update();
}

QPointF MapWidget::latLonToWorldPoint(double lat, double lon, int zoom) const
{
    double r_lat = lat * M_PI / 180.0;
    double n = std::pow(2.0, zoom);
    double x = n * ((lon + 180.0) / 360.0);
    double y = n * (1.0 - (std::log(std::tan(r_lat) + 1.0 / std::cos(r_lat)) / M_PI)) / 2.0;
    return QPointF(x, y);
}

void MapWidget::worldPointToLatLon(QPointF worldPoint, int zoom, double& lat, double& lon) const
{
    double n = std::pow(2.0, zoom);
    lon = worldPoint.x() / n * 360.0 - 180.0;
    double n_pi_y = M_PI - (2.0 * M_PI * worldPoint.y()) / n;
    lat = std::atan(std::sinh(n_pi_y)) * 180.0 / M_PI;
}

QString MapWidget::tileKey(int z, int x, int y) const
{
    return QString("%1_%2_%3").arg(z).arg(x).arg(y);
}

void MapWidget::updateVisibleTiles()
{
    int w = width();
    int h = height();
    if (w <= 0 || h <= 0) return;

    QPointF centerWorld = latLonToWorldPoint(m_centerLat, m_centerLon, m_zoom);
    int centerTileX = std::floor(centerWorld.x());
    int centerTileY = std::floor(centerWorld.y());

    // Вычисляем, сколько тайлов нужно по краям, чтобы закрыть весь экран
    // Один тайл = 256x256
    int tilesX = std::ceil((double)w / 256.0) / 2 + 1;
    int tilesY = std::ceil((double)h / 256.0) / 2 + 1;

    for (int dx = -tilesX; dx <= tilesX; ++dx) {
        for (int dy = -tilesY; dy <= tilesY; ++dy) {
            int tx = centerTileX + dx;
            int ty = centerTileY + dy;
            
            // Ограничения карты мира (от 0 до 2^z - 1)
            int maxTiles = std::pow(2, m_zoom);
            if (tx < 0 || tx >= maxTiles || ty < 0 || ty >= maxTiles) continue;

            QString key = tileKey(m_zoom, tx, ty);
            // Если тайла нет в кэше и он не в процессе скачивания
            if (!m_tileCache.contains(key) && !m_pendingRequests.contains(key)) {
                downloadTile(m_zoom, tx, ty);
            }
        }
    }
}

void MapWidget::downloadTile(int z, int x, int y)
{
    QString key = tileKey(z, x, y);
    // Используем красивые темные тайлы в стиле Dark Matter от CartoDB
    QString urlStr = QString("https://a.basemaps.cartocdn.com/dark_all/%1/%2/%3.png").arg(z).arg(x).arg(y);
    
    QNetworkRequest request((QUrl(urlStr)));
    request.setRawHeader("User-Agent", "MeteoriteWeatherApp/1.0");

    QNetworkReply* reply = m_networkManager->get(request);
    m_pendingRequests.insert(key, reply);

    connect(reply, &QNetworkReply::finished, this, [this, reply, key]() {
        m_pendingRequests.remove(key);
        if (reply->error() == QNetworkReply::NoError) {
            QPixmap pixmap;
            pixmap.loadFromData(reply->readAll());
            if (!pixmap.isNull()) {
                m_tileCache.insert(key, pixmap);
                update(); // Перерисовать карту, когда пришел кусочек
            }
        } else {
            qWarning() << "Tile download failed:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void MapWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    updateVisibleTiles();
}

void MapWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    
    // Рисуем базовый темный фон
    painter.fillRect(rect(), QColor(25, 25, 25));

    QPointF centerWorld = latLonToWorldPoint(m_centerLat, m_centerLon, m_zoom);
    int centerTileX = std::floor(centerWorld.x());
    int centerTileY = std::floor(centerWorld.y());
    
    // Смещение до миллиметра внутри центрального тайла (дробная часть от 0 до 1 * 256)
    double offsetX = (centerWorld.x() - centerTileX) * 256.0;
    double offsetY = (centerWorld.y() - centerTileY) * 256.0;

    int centerX = width() / 2;
    int centerY = height() / 2;

    int tilesX = std::ceil((double)width() / 256.0) / 2 + 1;
    int tilesY = std::ceil((double)height() / 256.0) / 2 + 1;

    for (int dx = -tilesX; dx <= tilesX; ++dx) {
        for (int dy = -tilesY; dy <= tilesY; ++dy) {
            int tx = centerTileX + dx;
            int ty = centerTileY + dy;
            
            int maxTiles = std::pow(2, m_zoom);
            if (tx < 0 || tx >= maxTiles || ty < 0 || ty >= maxTiles) continue;

            QString key = tileKey(m_zoom, tx, ty);
            
            int drawX = std::round(centerX - offsetX + dx * 256);
            int drawY = std::round(centerY - offsetY + dy * 256);

            if (m_tileCache.contains(key)) {
                // Если тайл загрузился — рисуем
                painter.drawPixmap(drawX, drawY, m_tileCache[key]);
            } else {
                // Пока грузится — рисуем серую сетку
                painter.setPen(QPen(QColor(50, 50, 50), 1, Qt::DashLine));
                painter.setBrush(Qt::NoBrush);
                painter.drawRect(drawX, drawY, 256, 256);
            }
        }
    }
}

void MapWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}

void MapWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDragging && (event->buttons() & Qt::LeftButton)) {
        int dx = event->pos().x() - m_lastMousePos.x();
        int dy = event->pos().y() - m_lastMousePos.y();

        QPointF wp = latLonToWorldPoint(m_centerLat, m_centerLon, m_zoom);
        
        // Смещаем координату X и Y 
        // 1 тайл = 256 пикселей.
        wp.rx() -= dx / 256.0;
        wp.ry() -= dy / 256.0;
        
        // Предотвращаем выход за пределы карты по оси Y (Северный и Южный полюса)
        double maxTiles = std::pow(2.0, m_zoom);
        wp.setY(std::clamp(wp.y(), 0.0, maxTiles - 1.0));
        
        // Делаем карту круглой по оси X (если ушел за край Земли, выходишь с другой стороны)
        if (wp.x() < 0) wp.setX(wp.x() + maxTiles);
        if (wp.x() >= maxTiles) wp.setX(wp.x() - maxTiles);

        // Переводим новые пиксели обратно в реальные Широту и Долготу
        worldPointToLatLon(wp, m_zoom, m_centerLat, m_centerLon);

        m_lastMousePos = event->pos();
        updateVisibleTiles();
        update();
    }
}

void MapWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        setCursor(Qt::ArrowCursor);
    }
}

void MapWidget::wheelEvent(QWheelEvent* event)
{
    int delta = event->angleDelta().y();
    if (delta > 0 && m_zoom < 19) {
        m_zoom++; // Приблизить
    } else if (delta < 0 && m_zoom > 2) {
        m_zoom--; // Отдалить
    }
    
    updateVisibleTiles();
    update();
}
