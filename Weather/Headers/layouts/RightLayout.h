#pragma once

#include <QVBoxLayout>      // Для вертикального расположения виджетов (основной макет)
#include <QLabel>           // Для отображения текста и иконок
#include <QWidget>
#include <QNetworkAccessManager> // Для отправки сетевых запросов (например, к OpenWeatherMap API)
#include <QNetworkReply>    // Для обработки ответов от сетевых запросов
#include <memory>           // Для использования std::unique_ptr для управления памятью
#include <vector>           // Для хранения списка указателей на QFrame (карточки прогноза)
#include <QFrame>           // Базовый класс для виджетов-контейнеров (используется для каждой карточки прогноза)
#include <QSvgRenderer>     // Для рендеринга SVG-изображений
#include <QPainter>         // Для рисования на QPixmap
#include <QScreen>          // Для получения информации о текущем экране (например, devicePixelRatio)
#include <QSet>             // Для хранения уникальных дней (чтобы не повторять прогноз на один и тот же день)
#include <QDebug>           // Для вывода отладочных сообщений (например, qWarning)
#include "Layouts.h"        // Ваш базовый класс для макетов (предполагается, что он существует)
#include "ForecastChartWidget.h"

class RightLayout : public QVBoxLayout, public Layouts
{
    Q_OBJECT // Макрос, необходимый для использования сигналов и слотов в Qt

public:
    // Конструктор класса. Принимает родительский виджет.
    explicit RightLayout(QWidget* parent = nullptr);
    // Переопределенная функция для создания и инициализации элементов макета.
    void createLayouts() override;
    // Функция для запроса данных о будущем прогнозе погоды для указанного города.
    void fetchFutureWeather(const QString& city);

signals:
    void coordinatesFetched(double lat, double lon);

public slots: // Объявление слотов (функций, которые реагируют на сигналы)
    // Слот, который будет вызван, когда QNetworkAccessManager завершит сетевой запрос для прогноза.
    void onForecastData(QNetworkReply* reply);

private:
    std::unique_ptr<QNetworkAccessManager> networkManager; // Менеджер для сетевых запросов.
    ForecastChartWidget* m_chartWidget = nullptr; // Виджет-график для температуры

    // Указатели на метки внутри дополнительных виджетов, чтобы обновлять их значения
    QLabel* m_feelsLikeValueLabel = nullptr;
    QLabel* m_visibilityValueLabel = nullptr;
    QLabel* m_sunriseValueLabel = nullptr;
    QLabel* m_sunsetValueLabel = nullptr;

    // Вспомогательный метод для создания стильных квадратных карточек показателей
    QWidget* createMetricCard(const QString& title, const QString& iconPath, QLabel*& valueLabelOut);

    // Функции для работы с SVG-иконками (скопированы из CenterLayout для переиспользования)
    QString getIconForWeather(const QString& description); // Возвращает путь к SVG-иконке по описанию погоды.
    QPixmap renderSvg(const QString& path , const QSize& baseSize, qreal devicePixelRatio = 1.0); // Рендерит SVG в QPixmap.
};
