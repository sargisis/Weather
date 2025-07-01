#pragma once

#include <QVBoxLayout>      // Для горизонтального расположения виджетов
#include <QLabel>           // Для отображения текста и изображений (иконок)
#include <QNetworkAccessManager> // Для отправки сетевых запросов (например, к OpenWeatherMap API)
#include <QNetworkReply>    // Для обработки ответов от сетевых запросов
#include <memory>           // Для использования std::unique_ptr для управления памятью
#include <vector>           // Для хранения списка QLabel*
#include <QSvgRenderer>     // Для рендеринга SVG-изображений
#include <QPainter>         // Для рисования на QPixmap
#include <QScreen>          // Для получения информации о текущем экране (например, devicePixelRatio)
#include <QTimeZone>        // Для работы с часовыми поясами (для отображения времени обновления)
#include "Layouts.h"        // Ваш базовый класс для макетов (предполагается, что он существует)

class CenterLayout : public QVBoxLayout, public Layouts
{
    Q_OBJECT // Макрос, необходимый для использования сигналов и слотов в Qt

public:
    // Конструктор класса. Принимает родительский виджет.
    explicit CenterLayout(QWidget* parent = nullptr);
    // Переопределенная функция для создания и инициализации элементов макета.
    void createLayouts() override;
    // Функция для запроса погодных данных для указанного города.
    void fetchWeatherDataForCity(const QString& city);

private slots:
    // Слот, который будет вызван, когда QNetworkAccessManager завершит сетевой запрос.
    void onWeatherDataReceived(QNetworkReply* reply);

private:
    // Вспомогательная функция для определения пути к SVG-иконке на основе описания погоды.
    QString getIconForWeather(const QString& description);
    // Вспомогательная функция для рендеринга SVG-изображения в QPixmap с учетом масштабирования для HiDPI.
    QPixmap renderSvg(const QString& path , const QSize& baseSize, qreal devicePixelRatio = 1.0);

    std::unique_ptr<QNetworkAccessManager> networkManager; // Менеджер для сетевых запросов.
    std::vector<QLabel*> weatherLabels; // Вектор указателей на QLabel для отображения текстовых данных о погоде (температура, описание, давление и т.д.).
    QLabel* weatherIcon = nullptr; // Указатель на QLabel для отображения главной иконки погоды.

signals:
    // Сигнал, который может быть испущен для уведомления других частей приложения об ошибке.
    // Пока не используется в этом файле, но полезен для централизованной обработки ошибок.
    void showError(const QString& message);
};
