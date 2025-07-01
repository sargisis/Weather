#pragma once

#include <QVBoxLayout>      // Для вертикального расположения виджетов (основной макет)
#include <QLabel>           // Для отображения текста и иконок
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

private slots: // Объявление слотов (функций, которые реагируют на сигналы)
    // Слот, который будет вызван, когда QNetworkAccessManager завершит сетевой запрос для прогноза.
    void onForecastData(QNetworkReply* reply);

private:
    std::unique_ptr<QNetworkAccessManager> networkManager; // Менеджер для сетевых запросов.
    std::vector<QFrame*> forecastCards; // Вектор указателей на QFrame, представляющие карточки прогноза.

    // Вспомогательные функции для управления карточками прогноза
    void clearCards(); // Очищает все существующие карточки прогноза из макета.
    QFrame* createForecastCard(const QString& time, double temp, const QString& desc); // Создает одну карточку прогноза.

    // Функции для работы с SVG-иконками (скопированы из CenterLayout для переиспользования)
    QString getIconForWeather(const QString& description); // Возвращает путь к SVG-иконке по описанию погоды.
    QPixmap renderSvg(const QString& path , const QSize& baseSize, qreal devicePixelRatio = 1.0); // Рендерит SVG в QPixmap.
};
