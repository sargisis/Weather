#include "RightLayout.h"
#include <QJsonDocument>    // Для работы с JSON-документами
#include <QJsonObject>      // Для работы с JSON-объектами
#include <QJsonArray>       // Для работы с JSON-массивами
#include <QDateTime>        // Для работы с датой и временем (для времени прогноза)
#include <QNetworkRequest>  // Для формирования сетевых запросов
#include <QLabel>           // Для отображения текста и иконок
#include <QHBoxLayout>      // Для горизонтального расположения виджетов внутри карточек
#include <QGuiApplication>  // Для доступа к информации о графическом приложении (devicePixelRatio)
#include <QSet>             // Для отслеживания уникальных дней (чтобы избежать дубликатов прогнозов)
#include <QDebug>           // Для вывода отладочных сообщений (qWarning)
#include <QSvgRenderer>     // Для рендеринга SVG-изображений
#include <QPainter>         // Для рисования на QPixmap
#include <QGridLayout>      // Для сетки виджетов под графиком
// Реализация функции renderSvg
// Эта функция берет путь к SVG-файлу, базовый размер и коэффициент масштабирования DPI
// и возвращает QPixmap, готовый для отображения на экране.
QPixmap RightLayout::renderSvg(const QString& path, const QSize& baseSize, qreal devicePixelRatio)
{
    // Определяем реальный размер пиксмапа, умножая базовый размер на коэффициент DPI.
    QSize actualSize = baseSize * devicePixelRatio;
    QPixmap pixmap(actualSize);
    pixmap.fill(Qt::transparent); // Заполняем pixmap полностью прозрачным цветом.

    QSvgRenderer renderer(path); // Создаем рендерер SVG из указанного пути.
    if (!renderer.isValid()) {
        qWarning() << "Failed to load SVG:" << path; // Выводим предупреждение, если SVG недействителен.
        return QPixmap(); // Возвращаем пустой QPixmap.
    }

    QPainter painter(&pixmap); // Создаем объект QPainter для рисования на pixmap.
    renderer.render(&painter); // Рендерим SVG-изображение на pixmap.

    // Устанавливаем devicePixelRatio для QPixmap. Это информирует Qt,
    // как масштабировать pixmap при его отображении, чтобы он выглядел правильно
    // на экранах с разным DPI (например, 1x, 2x, 3x).
    pixmap.setDevicePixelRatio(devicePixelRatio);

    return pixmap;
}

// Реализация функции getIconForWeather
// Эта функция сопоставляет описание погоды с соответствующим SVG-файлом иконки.
QString RightLayout::getIconForWeather(const QString& description)
{
    QString desc = description.toLower(); // Приводим описание к нижнему регистру для сравнения.
    if (desc.contains("snow")) return ":/icons/snowflake.svg";
    if (desc.contains("rain") || desc.contains("drizzle")) return ":/icons/cloud-rain.svg"; // Добавил "drizzle"
    if (desc.contains("storm") || desc.contains("thunder")) return ":/icons/cloud-lightning.svg";
    if (desc.contains("clear")) return ":/icons/sun.svg";
    if (desc.contains("cloud") || desc.contains("overcast") || desc.contains("scattered clouds")) return ":/icons/cloud.svg"; // Добавил "overcast" и "scattered clouds"
    if (desc.contains("fog") || desc.contains("mist") || desc.contains("haze") || desc.contains("smoke") || desc.contains("dust")) return ":/icons/cloud-fog.svg"; // Добавил больше туманных условий
    if (desc.contains("squalls") || desc.contains("tornado")) return ":/icons/wind.svg"; // Или что-то более подходящее для сильного ветра/шторма
    return ":/icons/thermometer-simple.svg"; // Запасная иконка, если совпадений не найдено.
}

// Конструктор RightLayout
RightLayout::RightLayout(QWidget* parent)
    : QVBoxLayout(parent), networkManager(std::make_unique<QNetworkAccessManager>(parent)) // Вызываем конструктор базового класса и инициализируем сетевой менеджер
{
    createLayouts(); // Вызываем функцию для создания и настройки макета.
    // Подключаем сигнал finished от networkManager к слоту onForecastData.
    // Это означает, что когда сетевой запрос будет завершен, вызовется наш слот.
    connect(networkManager.get(), &QNetworkAccessManager::finished, this, &RightLayout::onForecastData);
}

// Реализация создания карточек-метрик под графиком
QWidget* RightLayout::createMetricCard(const QString& title, const QString& iconPath, QLabel*& valueLabelOut)
{
    QWidget* card = new QWidget;
    card->setObjectName("metricCard");
    card->setStyleSheet(R"(
        #metricCard {
            background-color: #2d2d2d;
            border-radius: 12px;
        }
        QLabel {
            color: #ffffff;
            background: transparent;
        }
    )");

    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(10);

    // Верхняя часть: Иконка + Заголовок
    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(8);
    
    qreal screenPixelRatio = QGuiApplication::primaryScreen() ? QGuiApplication::primaryScreen()->devicePixelRatio() : 1.0;
    QLabel* iconLabel = new QLabel;
    iconLabel->setFixedSize(18, 18);
    iconLabel->setPixmap(renderSvg(iconPath, QSize(18, 18), screenPixelRatio));

    QLabel* titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("color: #aaaaaa; font-size: 13px; font-weight: bold;");

    topLayout->addWidget(iconLabel);
    topLayout->addWidget(titleLabel);
    topLayout->addStretch();
    
    // Нижняя часть: Значение
    valueLabelOut = new QLabel("—");
    valueLabelOut->setStyleSheet("font-size: 22px; font-weight: bold;");
    
    layout->addLayout(topLayout);
    layout->addWidget(valueLabelOut);
    
    return card;
}

// Реализация функции createLayouts для настройки внешнего вида макета
void RightLayout::createLayouts()
{
    setSpacing(15); // Устанавливаем отступ между элементами в вертикальном макете.
    setContentsMargins(10, 10, 10, 10); // Устанавливаем отступы от краев макета.
    setAlignment(Qt::AlignTop); // Выравниваем содержимое по верхнему краю.

    m_chartWidget = new ForecastChartWidget();
    this->addWidget(m_chartWidget);

    // Сетка 2x2 для нижних показателей
    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->setSpacing(15);
    
    QWidget* feelsLikeCard = createMetricCard("Feels Like", ":/icons/thermometer-simple.svg", m_feelsLikeValueLabel);
    QWidget* visibilityCard = createMetricCard("Visibility", ":/icons/cloud-fog.svg", m_visibilityValueLabel);
    QWidget* sunriseCard = createMetricCard("Sunrise", ":/icons/sun.svg", m_sunriseValueLabel);
    QWidget* sunsetCard = createMetricCard("Sunset", ":/icons/sun.svg", m_sunsetValueLabel); // Используем солнце, если луны вдруг нет

    gridLayout->addWidget(feelsLikeCard, 0, 0);
    gridLayout->addWidget(visibilityCard, 0, 1);
    gridLayout->addWidget(sunriseCard, 1, 0);
    gridLayout->addWidget(sunsetCard, 1, 1);

    this->addLayout(gridLayout);
    this->addStretch(); // Выталкиваем всё наверх, чтобы не растягивалось
}

// Функция для запроса данных о будущем прогнозе погоды для указанного города.
void RightLayout::fetchFutureWeather(const QString& city)
{
    QString apiKey = "716aa27058963431d3f2aafaeff2e033"; // Ваш API ключ OpenWeatherMap.
    QString url = QString("https://api.openweathermap.org/data/2.5/forecast?q=%1&appid=%2&units=metric")
                      .arg(city, apiKey); // Формируем URL для запроса.
    networkManager->get(QNetworkRequest(QUrl(url))); // Отправляем GET-запрос.
}

// Слот для обработки ответа от OpenWeatherMap API для прогноза погоды.
void RightLayout::onForecastData(QNetworkReply* reply)
{
    // Проверяем, была ли ошибка при получении ответа.
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Network error for forecast data:" << reply->errorString();
        m_chartWidget->updateData({}, {}, {}); // Очищаем график в случае ошибки
        reply->deleteLater();
        return;
    }

    // Если ошибок нет, парсим JSON-ответ.
    QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObject = json.object(); 

    if (!jsonObject.contains("list")) {
        qWarning() << "Invalid JSON response for forecast data: Missing 'list' field.";
        m_chartWidget->updateData({}, {}, {});
        reply->deleteLater();
        return;
    }

    QJsonArray list = jsonObject["list"].toArray(); 

    QVector<double> temps;
    QStringList times;
    QVector<QPixmap> icons;

    qreal screenPixelRatio = 1.0;
    if (QGuiApplication::primaryScreen()) {
        screenPixelRatio = QGuiApplication::primaryScreen()->devicePixelRatio();
    }

    // Берем прогноз на следующие 24 часа (8 элементов с шагом 3 часа)
    int maxPoints = std::min(8, (int)list.size());
    for (int i = 0; i < maxPoints; ++i) {
        auto entry = list[i].toObject();
        
        double temp = entry["main"].toObject()["temp"].toDouble();
        QString desc = entry["weather"].toArray()[0].toObject()["description"].toString();
        QString dt_txt = entry["dt_txt"].toString();
        
        QDateTime dateTime = QDateTime::fromString(dt_txt, "yyyy-MM-dd HH:mm:ss");
        QString timeStr = dateTime.toString("HH:mm");

        temps.append(temp);
        times.append(timeStr);
        icons.append(renderSvg(getIconForWeather(desc), QSize(24, 24), screenPixelRatio));
    }

    m_chartWidget->updateData(temps, times, icons);

    // Извлекаем "Feels Like" и "Visibility" из первого элемента списка
    if (!list.isEmpty()) {
        auto firstEntry = list[0].toObject();
        double feelsLike = firstEntry["main"].toObject()["feels_like"].toDouble();
        int visibility = firstEntry["visibility"].toInt();
        
        if (m_feelsLikeValueLabel) m_feelsLikeValueLabel->setText(QString::number(feelsLike, 'f', 1) + "°C");
        if (m_visibilityValueLabel) m_visibilityValueLabel->setText(QString::number(visibility / 1000.0, 'f', 1) + " km");
    }

    // Извлекаем Sunrise и Sunset из объекта city
    if (jsonObject.contains("city")) {
        auto cityObj = jsonObject["city"].toObject();
        qint64 sunriseTs = cityObj["sunrise"].toInteger();
        qint64 sunsetTs = cityObj["sunset"].toInteger();
        qint64 tzOffset = cityObj["timezone"].toInteger();

        QDateTime srTime = QDateTime::fromSecsSinceEpoch(sunriseTs, Qt::UTC).addSecs(tzOffset);
        QDateTime ssTime = QDateTime::fromSecsSinceEpoch(sunsetTs, Qt::UTC).addSecs(tzOffset);

        if (m_sunriseValueLabel) m_sunriseValueLabel->setText(srTime.toString("HH:mm"));
        if (m_sunsetValueLabel) m_sunsetValueLabel->setText(ssTime.toString("HH:mm"));
    }

    reply->deleteLater();
}


