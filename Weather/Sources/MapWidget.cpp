#include "MapWidget.h"
#include <QPainter>
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <cmath>
#include <QMouseEvent>
#include <QWheelEvent>
#include <algorithm>
#include <QGeoCoordinate>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <QLineEdit>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPainterPath>
#include <QTimer>

MapWidget::MapWidget(QWidget* parent)
    : QWidget(parent), m_centerLat(40.1811), m_centerLon(44.5136), m_zoom(11) // Default: Yerevan
{
    // Оптимизация: Qt не будет чистить фон перед рисованием (мы рисуем всё сами)
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
    m_networkManager = new QNetworkAccessManager(this);

    // --- Создаем UI элементы (поля ввода и кнопка) ---
    m_fromInput = new QLineEdit(this);
    m_fromInput->setPlaceholderText("From (e.g. London)");
    m_fromInput->setStyleSheet("background-color: #2e2e2e; color: white; padding: 10px; border-radius: 8px; font-size: 14px; border: 1px solid #555;");

    m_toInput = new QLineEdit(this);
    m_toInput->setPlaceholderText("To (e.g. Paris)");
    m_toInput->setStyleSheet("background-color: #2e2e2e; color: white; padding: 10px; border-radius: 8px; font-size: 14px; border: 1px solid #555;");

    m_routeBtn = new QPushButton("Build Route", this);
    m_routeBtn->setStyleSheet("background-color: #007bff; color: white; padding: 10px; border-radius: 8px; font-weight: bold; font-size: 14px;");
    m_routeBtn->setCursor(Qt::PointingHandCursor);

    connect(m_routeBtn, &QPushButton::clicked, this, &MapWidget::onRouteButtonClicked);

    // --- Живой поиск (autocomplete) ---
    auto setupAutocomplete = [this](QLineEdit* input) {
        connect(input, &QLineEdit::textEdited, this, [this, input](const QString& text) {
            if (text.length() < 2) return;
            QString url = QString("https://nominatim.openstreetmap.org/search?q=%1&format=json&limit=5").arg(text);
            QNetworkRequest req((QUrl(url)));
            req.setRawHeader("User-Agent", "MeteoriteWeatherApp/1.0");
            auto* reply = m_networkManager->get(req);
            connect(reply, &QNetworkReply::finished, this, [this, input, reply]() {
                if (reply->error() != QNetworkReply::NoError) { reply->deleteLater(); return; }
                QJsonArray arr = QJsonDocument::fromJson(reply->readAll()).array();
                QStringList suggestions;
                for (const auto& item : arr) {
                    suggestions << item.toObject()["display_name"].toString().split(",").first();
                }
                QCompleter* completer = new QCompleter(suggestions, input);
                completer->setCaseSensitivity(Qt::CaseInsensitive);
                completer->setMaxVisibleItems(5);
                input->setCompleter(completer);
                completer->complete();
                reply->deleteLater();
            });
        });
    };
    setupAutocomplete(m_fromInput);
    setupAutocomplete(m_toInput);

    // Дебонсинг загрузки тайлов при панорамировании
    m_tileLoadTimer = new QTimer(this);
    m_tileLoadTimer->setSingleShot(true);
    m_tileLoadTimer->setInterval(80); // 80мс задержка после остановки
    connect(m_tileLoadTimer, &QTimer::timeout, this, &MapWidget::updateVisibleTiles);
}

// ... 
void MapWidget::onRouteButtonClicked()
{
    calculateRoute(m_fromInput->text(), m_toInput->text());
}

void MapWidget::calculateRoute(const QString& fromCity, const QString& toCity)
{
    if (fromCity.isEmpty() || toCity.isEmpty()) return;

    QString apiKey = "716aa27058963431d3f2aafaeff2e033"; // OpenWeather API

    // 1. Получаем координаты первого города
    QString url1 = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2").arg(fromCity, apiKey);
    QNetworkReply* reply1 = m_networkManager->get(QNetworkRequest(QUrl(url1)));
    
    connect(reply1, &QNetworkReply::finished, this, [this, reply1, toCity, apiKey]() {
        if (reply1->error() != QNetworkReply::NoError) { reply1->deleteLater(); return; }
        QJsonObject root1 = QJsonDocument::fromJson(reply1->readAll()).object();
        double lat1 = root1["coord"].toObject()["lat"].toDouble();
        double lon1 = root1["coord"].toObject()["lon"].toDouble();
        reply1->deleteLater();

        // 2. Получаем координаты второго города
        QString url2 = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2").arg(toCity, apiKey);
        QNetworkReply* reply2 = m_networkManager->get(QNetworkRequest(QUrl(url2)));
        connect(reply2, &QNetworkReply::finished, this, [this, reply2, lat1, lon1]() {
            if (reply2->error() != QNetworkReply::NoError) { reply2->deleteLater(); return; }
            QJsonObject root2 = QJsonDocument::fromJson(reply2->readAll()).object();
            double lat2 = root2["coord"].toObject()["lat"].toDouble();
            double lon2 = root2["coord"].toObject()["lon"].toDouble();
            reply2->deleteLater();

            // 3. Строим маршрут через OSRM
            requestOSRMRoute(lat1, lon1, lat2, lon2);
        });
    });
}

void MapWidget::requestOSRMRoute(double lat1, double lon1, double lat2, double lon2)
{
    // OSRM формат: lon,lat;lon,lat
    QString osrmUrl = QString("http://router.project-osrm.org/route/v1/driving/%1,%2;%3,%4?overview=full&geometries=polyline")
                      .arg(lon1).arg(lat1).arg(lon2).arg(lat2);
                      
    QNetworkRequest req((QUrl(osrmUrl)));
    req.setHeader(QNetworkRequest::UserAgentHeader, "MeteoriteWeatherApp/1.0");

    QNetworkReply* reply = m_networkManager->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply, lat1, lon1, lat2, lon2]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonObject root = QJsonDocument::fromJson(reply->readAll()).object();
            auto routeObj = root["routes"].toArray()[0].toObject();
            QString encodedGeometry = routeObj["geometry"].toString();

            m_routeDistance = routeObj["distance"].toDouble();
            m_routeDuration = routeObj["duration"].toDouble();
            m_routeStart = QPointF(lat1, lon1);
            m_routeEnd = QPointF(lat2, lon2);
            m_hasRoute = true;

            m_routePath = decodePolyline(encodedGeometry);
            m_cachedRoutePath = QPainterPath(); // сбрасываем кеш
            m_cachedRouteZoom = -1;

            m_centerLat = (lat1 + lat2) / 2.0;
            m_centerLon = (lon1 + lon2) / 2.0;

            // --- Авто-зум: подбираем зум чтобы маршрут влез полностью ---
            if (!m_routePath.isEmpty()) {
                double minLat = 1e9, maxLat = -1e9, minLon = 1e9, maxLon = -1e9;
                for (const QPointF& pt : m_routePath) {
                    minLat = std::min(minLat, pt.x());
                    maxLat = std::max(maxLat, pt.x());
                    minLon = std::min(minLon, pt.y());
                    maxLon = std::max(maxLon, pt.y());
                }
                double dLat = maxLat - minLat;
                double dLon = maxLon - minLon;
                int w = std::max(width(), 400);
                int h = std::max(height(), 300);
                // Берём зум при котором bounding box помещается в 80% экрана
                int zoomLat = (dLat > 0) ? (int)std::floor(std::log2(h * 0.8 / 256.0 * 360.0 / dLat)) : 10;
                int zoomLon = (dLon > 0) ? (int)std::floor(std::log2(w * 0.8 / 256.0 * 360.0 / dLon)) : 10;
                m_zoom = std::max(2, std::min({zoomLat, zoomLon, 15}));
            }

            updateVisibleTiles();
            update();
        } else {
            qWarning() << "OSRM Routing failed:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

QList<QPointF> MapWidget::decodePolyline(const QString& encoded) const
{
    QList<QPointF> path;
    int index = 0, len = encoded.length();
    int lat = 0, lng = 0;

    while (index < len) {
        int b, shift = 0, result = 0;
        do {
            b = encoded.at(index++).unicode() - 63;
            result |= (b & 0x1f) << shift;
            shift += 5;
        } while (b >= 0x20);
        int dlat = ((result & 1) ? ~(result >> 1) : (result >> 1));
        lat += dlat;

        shift = 0;
        result = 0;
        do {
            b = encoded.at(index++).unicode() - 63;
            result |= (b & 0x1f) << shift;
            shift += 5;
        } while (b >= 0x20);
        int dlng = ((result & 1) ? ~(result >> 1) : (result >> 1));
        lng += dlng;

        path.append(QPointF(lat / 1e5, lng / 1e5));
    }
    return path;
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

void MapWidget::setRoute(const Route& route)
{
    // Конвертируем QGeoCoordinate в QPointF (lat, lon) для отрисовки
    m_routePath.clear();
    for (const QGeoCoordinate& coord : route.geometry) {
        m_routePath.append(QPointF(coord.latitude(), coord.longitude()));
    }

    if (!route.geometry.isEmpty()) {
        // Центрируем между началом и концом
        const QGeoCoordinate& a = route.geometry.first();
        const QGeoCoordinate& b = route.geometry.last();
        m_centerLat = (a.latitude()  + b.latitude())  / 2.0;
        m_centerLon = (a.longitude() + b.longitude()) / 2.0;
        m_zoom = 6;
        updateVisibleTiles();
    }
    update();
}

void MapWidget::updateGpsPosition(const QGeoCoordinate& pos)
{
    m_gpsPos = pos;
    m_hasGps = true;
    update(); // перерисовываем с маркером
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

    // Позиционируем поля ввода в правом верхнем углу
    int padding = 20;
    int inputWidth = 200;
    int inputHeight = 40;
    
    m_fromInput->setGeometry(width() - inputWidth - padding, padding, inputWidth, inputHeight);
    m_toInput->setGeometry(width() - inputWidth - padding, padding + inputHeight + 10, inputWidth, inputHeight);
    m_routeBtn->setGeometry(width() - inputWidth - padding, padding + (inputHeight + 10) * 2, inputWidth, inputHeight);
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

    // === Маршрут, маркеры и HUD: рисуем ТОЛЬКО когда не движемся ===
    // Во время drag рисуем только тайлы — даёт 0 лагов при панорамировании
    if (!m_isDragging) {

    // --- Маршрут OSRM ---
    if (!m_routePath.isEmpty()) {
        // Пересчитываем путь только если изменился зум
        // (центр не влияет на world-координаты, только на transform)
        if (m_cachedRouteZoom != m_zoom) {
            m_cachedRoutePath = QPainterPath();
            bool first = true;
            for (const QPointF& pt : m_routePath) {
                QPointF worldPt = latLonToWorldPoint(pt.x(), pt.y(), m_zoom);
                if (first) { m_cachedRoutePath.moveTo(worldPt); first = false; }
                else { m_cachedRoutePath.lineTo(worldPt); }
            }
            m_cachedRouteZoom = m_zoom;
        }

        // Применяем трансформацию НА PAINTER, а не на путь
        // Это O(1) — не создаём новый QPainterPath на каждый кадр!
        QTransform t;
        t.translate(centerX - centerWorld.x() * 256.0,
                    centerY - centerWorld.y() * 256.0);
        t.scale(256.0, 256.0);

        painter.save();
        painter.setTransform(t);
        painter.setRenderHint(QPainter::Antialiasing, !m_isDragging);
        painter.setPen(QPen(QColor(0, 120, 255, 60), 12.0 / 256.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPath(m_cachedRoutePath);
        painter.setPen(QPen(QColor(0, 160, 255, 220), 5.0 / 256.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPath(m_cachedRoutePath);
        painter.restore();

        // --- Маркеры A и B (не рисуем во время драга) ---
        if (!m_isDragging && m_hasRoute) {
            auto drawPin = [&](double lat, double lon, const QString& label, QColor color) {
                QPointF wpt = latLonToWorldPoint(lat, lon, m_zoom);
                double px = centerX + (wpt.x() - centerWorld.x()) * 256.0;
                double py = centerY + (wpt.y() - centerWorld.y()) * 256.0;
                painter.setPen(Qt::NoPen);
                painter.setBrush(QColor(0, 0, 0, 80));
                painter.drawEllipse(QPointF(px + 2, py + 2), 14, 14);
                painter.setBrush(color);
                painter.drawEllipse(QPointF(px, py), 14, 14);
                painter.setPen(QPen(Qt::white, 2));
                painter.setBrush(Qt::NoBrush);
                painter.drawEllipse(QPointF(px, py), 14, 14);
                painter.setPen(Qt::white);
                QFont f = painter.font();
                f.setBold(true); f.setPixelSize(13);
                painter.setFont(f);
                painter.drawText(QRectF(px - 14, py - 14, 28, 28), Qt::AlignCenter, label);
            };
            drawPin(m_routeStart.x(), m_routeStart.y(), "A", QColor(0, 180, 80));
            drawPin(m_routeEnd.x(), m_routeEnd.y(), "B", QColor(220, 50, 50));
        }
    }

    // --- Рисуем GPS маркер (синяя точка с белой окантовкой) ---
    if (m_hasGps && m_gpsPos.isValid()) {
        QPointF gpsWorld = latLonToWorldPoint(m_gpsPos.latitude(), m_gpsPos.longitude(), m_zoom);
        double gpsX = centerX + (gpsWorld.x() - centerWorld.x()) * 256.0;
        double gpsY = centerY + (gpsWorld.y() - centerWorld.y()) * 256.0;
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255, 255, 255, 230));
        painter.drawEllipse(QPointF(gpsX, gpsY), 10, 10);
        painter.setBrush(QColor(0, 120, 255));
        painter.drawEllipse(QPointF(gpsX, gpsY), 7, 7);
    }

    // --- HUD панель внизу (только когда есть маршрут) ---
    if (m_hasRoute) {
        // Форматируем расстояние
        QString distStr = m_routeDistance >= 1000
            ? QString("%1 км").arg(m_routeDistance / 1000.0, 0, 'f', 0)
            : QString("%1 м").arg((int)m_routeDistance);

        // Форматируем время
        int totalMin = (int)(m_routeDuration / 60);
        QString timeStr = totalMin >= 60
            ? QString("%1 ч %2 мин").arg(totalMin / 60).arg(totalMin % 60)
            : QString("%1 мин").arg(totalMin);

        // Фон HUD
        int hudH = 70;
        int hudW = width();
        QRect hud(0, height() - hudH, hudW, hudH);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(15, 15, 20, 220));
        painter.drawRect(hud);

        // Разделитель сверху
        painter.setPen(QPen(QColor(0, 160, 255, 100), 1));
        painter.drawLine(0, height() - hudH, hudW, height() - hudH);

        // ----- Иконка навигации -----
        painter.setPen(QColor(0, 160, 255));
        QFont iconFont;
        iconFont.setPixelSize(28);
        painter.setFont(iconFont);
        painter.drawText(QRect(20, height() - hudH, 50, hudH), Qt::AlignVCenter | Qt::AlignHCenter, "→");

        // ----- Расстояние (крупно) -----
        QFont bigFont;
        bigFont.setPixelSize(24);
        bigFont.setBold(true);
        painter.setFont(bigFont);
        painter.setPen(Qt::white);
        painter.drawText(QRect(80, height() - hudH, 160, hudH / 2), Qt::AlignVCenter, distStr);

        // ----- Время (мелко под расстоянием) -----
        QFont smallFont;
        smallFont.setPixelSize(14);
        painter.setFont(smallFont);
        painter.setPen(QColor(170, 170, 170));
        painter.drawText(QRect(80, height() - hudH / 2, 160, hudH / 2), Qt::AlignVCenter, timeStr);

        // ----- Названия городов справа -----
        QFont cityFont;
        cityFont.setPixelSize(13);
        painter.setFont(cityFont);
        painter.setPen(QColor(200, 200, 200));
        QString routeLabel = m_fromInput->text() + " → " + m_toInput->text();
        painter.drawText(QRect(hudW / 2, height() - hudH, hudW / 2 - 20, hudH),
                         Qt::AlignVCenter | Qt::AlignRight, routeLabel);
    }

    } // конец блока !m_isDragging
}

void MapWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    else if (event->button() == Qt::RightButton) {
        // Конвертируем пиксели клика в lat/lon
        QPointF centerWorld = latLonToWorldPoint(m_centerLat, m_centerLon, m_zoom);
        int cx = width() / 2, cy = height() / 2;
        double wx = centerWorld.x() + (event->pos().x() - cx) / 256.0;
        double wy = centerWorld.y() + (event->pos().y() - cy) / 256.0;
        double clickLat, clickLon;
        worldPointToLatLon(QPointF(wx, wy), m_zoom, clickLat, clickLon);

        // Геокодируем точку через Nominatim
        QString url = QString("https://nominatim.openstreetmap.org/reverse?lat=%1&lon=%2&format=json")
                      .arg(clickLat, 0, 'f', 6).arg(clickLon, 0, 'f', 6);
        QNetworkRequest req((QUrl(url)));
        req.setRawHeader("User-Agent", "MeteoriteWeatherApp/1.0");
        auto* reply = m_networkManager->get(req);

        // ВАЖНО: копируем pos() в QPoint — event уничтожится до прихода ответа!
        QPoint clickPos = event->pos();

        connect(reply, &QNetworkReply::finished, this, [this, reply, clickLat, clickLon, clickPos]() {
            QString placeName;
            if (reply->error() == QNetworkReply::NoError) {
                QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
                // Берём город или район
                QJsonObject addr = obj["address"].toObject();
                placeName = addr["city"].toString();
                if (placeName.isEmpty()) placeName = addr["town"].toString();
                if (placeName.isEmpty()) placeName = addr["village"].toString();
                if (placeName.isEmpty()) placeName = addr["county"].toString();
                if (placeName.isEmpty()) placeName = obj["display_name"].toString().split(",").first();
            }
            if (placeName.isEmpty()) placeName = QString("%1,%2").arg(clickLat, 0,'f',4).arg(clickLon,0,'f',4);
            reply->deleteLater();

            // Контекстное меню
            QMenu* menu = new QMenu(this);
            menu->setStyleSheet(R"(
                QMenu { background:#1e1e2a; color:white; border:1px solid #333; border-radius:8px; padding:4px; }
                QMenu::item { padding:8px 20px; border-radius:6px; }
                QMenu::item:selected { background:#0078ff; }
            )");

            QAction* fromAct = menu->addAction(QString("🟢 Отсюда: %1").arg(placeName));
            QAction* toAct   = menu->addAction(QString("🔴 Ехать сюда: %1").arg(placeName));
            menu->addSeparator();
            QAction* centerAct = menu->addAction("📍 Центрировать карту здесь");

            connect(fromAct,   &QAction::triggered, this, [this, placeName]{ m_fromInput->setText(placeName); });
            connect(toAct,     &QAction::triggered, this, [this, placeName]{ m_toInput->setText(placeName); });
            connect(centerAct, &QAction::triggered, this, [this, clickLat = clickLat, clickLon = clickLon] {
                setCenter(clickLat, clickLon, m_zoom);
            });

            menu->exec(mapToGlobal(clickPos));
            menu->deleteLater();
        });
    }
}

void MapWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDragging && (event->buttons() & Qt::LeftButton)) {
        int dx = event->pos().x() - m_lastMousePos.x();
        int dy = event->pos().y() - m_lastMousePos.y();

        QPointF wp = latLonToWorldPoint(m_centerLat, m_centerLon, m_zoom);
        wp.rx() -= dx / 256.0;
        wp.ry() -= dy / 256.0;

        double maxTiles = std::pow(2.0, m_zoom);
        wp.setY(std::clamp(wp.y(), 0.0, maxTiles - 1.0));
        if (wp.x() < 0) wp.setX(wp.x() + maxTiles);
        if (wp.x() >= maxTiles) wp.setX(wp.x() - maxTiles);

        worldPointToLatLon(wp, m_zoom, m_centerLat, m_centerLon);

        m_lastMousePos = event->pos();

        // Перерисовываем немедленно для плавности
        update();

        // Загружаем тайлы с задержкой (дебонсинг)
        m_tileLoadTimer->start();
    }
}

void MapWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        setCursor(Qt::ArrowCursor);
        // Перерисовываем полностью с маршрутом после опуска кнопки
        update();
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
