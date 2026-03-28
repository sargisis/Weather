#pragma once

#include <QWidget>
#include <QVector>
#include <QStringList>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>

class ForecastChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit ForecastChartWidget(QWidget* parent = nullptr);
    void updateData(const QVector<double>& temps, const QStringList& times, const QVector<QPixmap>& icons);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<double> m_temps;
    QStringList m_times;
    QVector<QPixmap> m_icons;
};
