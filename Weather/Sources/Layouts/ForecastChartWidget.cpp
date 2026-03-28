#include "../../Headers/layouts/ForecastChartWidget.h"
#include <QLinearGradient>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QDebug>
#include <algorithm>

ForecastChartWidget::ForecastChartWidget(QWidget* parent) : QWidget(parent) {
    setMinimumHeight(250);
}

void ForecastChartWidget::updateData(const QVector<double>& temps, const QStringList& times, const QVector<QPixmap>& icons) {
    m_temps = temps;
    m_times = times;
    m_icons = icons;
    update(); // Перерисовать виджет
}

void ForecastChartWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    if (m_temps.isEmpty()) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int marginX = 25; // Отступы по краям
    int marginY = 40;
    int bottomTextMargin = 50; 
    
    int w = width() - 2 * marginX;
    int h = height() - marginY - bottomTextMargin;

    // Ищем минимум и максимум для масштабирования
    double minTemp = *std::min_element(m_temps.constBegin(), m_temps.constEnd());
    double maxTemp = *std::max_element(m_temps.constBegin(), m_temps.constEnd());
    
    // Добавляем запас
    if (maxTemp - minTemp < 1.0) {
        maxTemp += 1.0;
        minTemp -= 1.0;
    }
    double tempRange = maxTemp - minTemp;

    int numPoints = m_temps.size();
    double stepX = w / (double)std::max(1, numPoints - 1);

    QVector<QPointF> points;
    for (int i = 0; i < numPoints; ++i) {
        double x = marginX + i * stepX;
        double y = marginY + h - ((m_temps[i] - minTemp) / tempRange) * h;
        points.append(QPointF(x, y));
    }

    // Строим кривую (spline)
    QPainterPath path;
    if (!points.isEmpty()) {
        path.moveTo(points.first());
        for (int i = 0; i < points.size() - 1; ++i) {
            QPointF p1 = points[i];
            QPointF p2 = points[i+1];
            
            double xDiff = (p2.x() - p1.x()) / 2.0;
            QPointF c1(p1.x() + xDiff, p1.y());
            QPointF c2(p2.x() - xDiff, p2.y());
            
            path.cubicTo(c1, c2, p2);
        }
    }

    // Заливаем полигон под графиком красивым градиентом
    QPainterPath fillPath = path;
    fillPath.lineTo(points.last().x(), height() - bottomTextMargin);
    fillPath.lineTo(points.first().x(), height() - bottomTextMargin);
    fillPath.closeSubpath();

    QLinearGradient fillGradient(0, marginY, 0, height() - bottomTextMargin);
    fillGradient.setColorAt(0.0, QColor(0, 150, 255, 120)); // Полупрозрачный неоновый синий
    fillGradient.setColorAt(1.0, QColor(0, 150, 255, 0));   // Полностью прозрачный снизу
    painter.fillPath(fillPath, QBrush(fillGradient));

    // Рисуем саму кривую
    QPen linePen(QColor(0, 150, 255), 3);
    painter.setPen(linePen);
    painter.drawPath(path);

    // Рисуем точки, температуру и подписи (время, иконки)
    QFont font = painter.font();
    font.setPointSize(9); // Сделаем шрифт чуть меньше для компактности
    font.setBold(true);
    painter.setFont(font);

    for (int i = 0; i < numPoints; ++i) {
        // Рисуем узел
        painter.setBrush(QColor(255, 255, 255));
        painter.setPen(QColor(0, 150, 255));
        painter.drawEllipse(points[i], 4, 4);

        // Рисуем температуру (текст)
        painter.setPen(QColor(255, 255, 255));
        QString tempStr = QString::number(m_temps[i], 'f', 0) + "°";
        int textW = painter.fontMetrics().horizontalAdvance(tempStr);
        painter.drawText(points[i].x() - textW / 2, points[i].y() - 10, tempStr);

        // Рисуем иконочку внизу оси X
        if (i < m_icons.size() && !m_icons[i].isNull()) {
            int iconSize = 22;
            painter.drawPixmap(points[i].x() - iconSize / 2, height() - bottomTextMargin + 5, iconSize, iconSize, m_icons[i]);
        }

        // Подписываем время
        // Чтобы текст не слипался на узких экранах, мы скрываем каждый второй элемент.
        // Но при расширении окна (когда шаг больше 50px), мы показываем все часы!
        bool shouldDrawTime = true;
        if (stepX < 50.0 && (i % 2 != 0)) {
            shouldDrawTime = false;
        }

        if (i < m_times.size() && shouldDrawTime) {
            painter.setPen(QColor(150, 150, 150)); // Сделаем текст времени чуть бледнее
            QFont timeFont = painter.font();
            timeFont.setPointSize(8);
            timeFont.setBold(false);
            painter.setFont(timeFont);

            QString timeStr = m_times[i];
            int tw = painter.fontMetrics().horizontalAdvance(timeStr);
            painter.drawText(points[i].x() - tw / 2, height() - 5, timeStr);

            painter.setFont(font); // Возвращаем жирный шрифт для следующей температуры
        }
    }
}
