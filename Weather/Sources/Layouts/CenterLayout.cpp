#include "CenterLayout.h"
#include <QJsonDocument>    // Для работы с JSON-документами
#include <QJsonObject>      // Для работы с JSON-объектами
#include <QJsonArray>       // Для работы с JSON-массивами
#include <QDateTime>        // Для работы с датой и временем
#include <QFrame>           // Базовый класс для виджетов-контейнеров (используется для карточки погоды)
#include <QNetworkRequest>  // Для формирования сетевых запросов
#include <QLabel>           // Для отображения текста и изображений
#include <QGuiApplication>  // Для доступа к информации о графическом приложении (например, первичный экран)
#include <QTimeZone>        // Для работы с часовыми поясами (для корректного отображения времени обновления)
#include <QDebug>           // Для вывода отладочных сообщений (например, qWarning)

// Реализация функции renderSvg
// Эта функция берет путь к SVG-файлу, базовый размер и коэффициент масштабирования DPI
// и возвращает QPixmap, готовый для отображения на экране.
QPixmap CenterLayout::renderSvg(const QString& path, const QSize& baseSize, qreal devicePixelRatio)
{
    // Определяем реальный размер пиксмапа, умножая базовый размер на коэффициент DPI.
    // Это гарантирует, что иконка будет четкой на экранах с высоким разрешением (Retina).
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

// Конструктор CenterLayout
CenterLayout::CenterLayout(QWidget* parent)
    : QVBoxLayout(parent), // Вызываем конструктор базового класса QVBoxLayout
    networkManager(std::make_unique<QNetworkAccessManager>(parent)) // Инициализируем QNetworkAccessManager
{
    createLayouts(); // Вызываем функцию для создания всех элементов интерфейса.
    // Подключаем сигнал finished от networkManager к слоту onWeatherDataReceived.
    // Это означает, что когда сетевой запрос будет завершен, вызовется наш слот.
    connect(networkManager.get(), &QNetworkAccessManager::finished, this, &CenterLayout::onWeatherDataReceived);
}

// Реализация функции createLayouts для создания визуальных компонентов макета
void CenterLayout::createLayouts()
{
    auto* card = new QFrame; // Создаем фрейм, который будет служить фоновой карточкой для погоды.
    card->setObjectName("weatherCard"); // Устанавливаем objectName для применения стилей через CSS.

    auto* layout = new QVBoxLayout(card); // Создаем вертикальный макет внутри карточки.
    layout->setContentsMargins(20, 20, 20, 20); // Отступы от края карточки.
    layout->setSpacing(10); // Пространство между виджетами внутри макета.

    // Применяем CSS-стили к карточке и внутренним QLabel.
    card->setStyleSheet(R"(
        #weatherCard {
            background-color: #222;
            border-radius: 20px;
        }
        QLabel {
            color: white;
            font-size: 16px;
            background: transparent;
        }
        #tempLabel {
            font-size: 42px;
            font-weight: bold;
            qproperty-alignment: AlignCenter;
        }
        #descLabel {
            font-size: 16px;
            color: #aaa;
            qproperty-alignment: AlignCenter;
        }
    )");

    weatherIcon = new QLabel; // Создаем QLabel для отображения главной иконки погоды.
    weatherIcon->setFixedSize(40, 40); // Устанавливаем фиксированный размер для иконки.
    weatherIcon->setAlignment(Qt::AlignCenter); // Выравниваем иконку по центру.
    weatherIcon->setStyleSheet("background: transparent;"); // Делаем фон прозрачным.
    layout->addWidget(weatherIcon); // Добавляем иконку в макет.

    QLabel* tempLabel = new QLabel("—°C"); // Создаем QLabel для температуры.
    tempLabel->setObjectName("tempLabel"); // Устанавливаем objectName для стилей.

    QLabel* descLabel = new QLabel("Description —"); // Создаем QLabel для описания погоды.
    descLabel->setObjectName("descLabel"); // Устанавливаем objectName для стилей.

    layout->addWidget(tempLabel); // Добавляем метку температуры.
    layout->addWidget(descLabel); // Добавляем метку описания.

    // Списки имен SVG-иконок и начальных текстов для параметров погоды.
    QStringList iconNames = {"feather", "drop", "wind", "clock"};
    QStringList paramTexts = {
        "Pressure: —",
        "Humidity: —",
        "Wind Speed: —",
        "Updated at: —"
    };

    // Получаем текущий devicePixelRatio экрана для корректного рендеринга SVG.
    qreal screenPixelRatio = 1.0;
    if (QGuiApplication::primaryScreen()) {
        screenPixelRatio = QGuiApplication::primaryScreen()->devicePixelRatio();
    }

    // Создаем строки для отображения давления, влажности, скорости ветра и времени обновления.
    for (int i = 0; i < paramTexts.size(); ++i) {
        auto* row = new QWidget; // Создаем виджет-контейнер для каждой строки.
        auto* rowLayout = new QHBoxLayout(row); // Горизонтальный макет для иконки и текста.
        rowLayout->setContentsMargins(0, 0, 0, 0); // Без отступов.
        rowLayout->setSpacing(10); // Пространство между иконкой и текстом.

        QLabel* iconLabel = new QLabel; // Метка для маленькой иконки параметра.
        iconLabel->setFixedSize(20, 20); // Фиксированный размер для иконки.
        // Рендерим SVG-иконку с учетом DPI.
        iconLabel->setPixmap(renderSvg(QString(":/icons/%1.svg").arg(iconNames[i]), QSize(20, 20), screenPixelRatio));
        iconLabel->setStyleSheet("background: transparent;"); // Прозрачный фон.

        QLabel* textLabel = new QLabel(paramTexts[i]); // Метка для текста параметра.
        textLabel->setStyleSheet("background: transparent;"); // Прозрачный фон.
        rowLayout->addWidget(iconLabel); // Добавляем иконку в строку.
        rowLayout->addWidget(textLabel); // Добавляем текст в строку.

        layout->addWidget(row); // Добавляем всю строку в основной вертикальный макет.
        weatherLabels.push_back(textLabel); // Добавляем метку в вектор для последующего обновления.
    }

    // Вставляем метки температуры и описания в начало вектора weatherLabels,
    // чтобы их порядок соответствовал порядку вывода в onWeatherDataReceived.
    weatherLabels.insert(weatherLabels.begin(), descLabel);
    weatherLabels.insert(weatherLabels.begin(), tempLabel);

    this->addWidget(card); // Добавляем главную карточку погоды в макет CenterLayout.
}

// Функция для получения погодных данных от OpenWeatherMap API.
void CenterLayout::fetchWeatherDataForCity(const QString& city)
{
    QString apiKey = "716aa27058963431d3f2aafaeff2e033"; // Ваш API ключ OpenWeatherMap.
    QString url = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric")
                      .arg(city, apiKey); // Формируем URL для запроса.
    networkManager->get(QNetworkRequest(QUrl(url))); // Отправляем GET-запрос.
}

// Слот для обработки ответа от OpenWeatherMap API.
void CenterLayout::onWeatherDataReceived(QNetworkReply* reply)
{
    // Проверяем, была ли ошибка при получении ответа.
    if (reply->error() != QNetworkReply::NoError) {
        QString errorMessage;
        // Получаем HTTP статус-код ответа.
        int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        // Если статус-код 404 (Not Found), значит, город не найден.
        if (httpStatus == 404) {
            errorMessage = "City not found. Please enter a valid city name.";
            qWarning() << "City not found for weather data. HTTP 404."; // Логируем предупреждение.
        } else {
            // Для всех других сетевых ошибок.
            errorMessage = "Failed to load current weather data: " + reply->errorString();
            qWarning() << "Network error for weather data:" << reply->errorString(); // Логируем предупреждение.
        }

        // Обновляем метки интерфейса, чтобы показать сообщение об ошибке.
        if (!weatherLabels.empty()) {
            weatherLabels[0]->setText("—°C"); // Сбрасываем температуру.
            weatherLabels[1]->setText(errorMessage); // Выводим сообщение об ошибке.
            // Очищаем или сбрасываем остальные метки, чтобы не показывать старые/некорректные данные.
            for(size_t i = 2; i < weatherLabels.size(); ++i) {
                weatherLabels[i]->setText("—");
            }
        }
        // Очищаем или сбрасываем иконку погоды.
        if (weatherIcon) {
            weatherIcon->setPixmap(QPixmap()); // Очищаем иконку.
        }

        reply->deleteLater(); // Удаляем объект QNetworkReply после использования.
        return; // Выходим из функции, так как произошла ошибка.
    }

    // Если ошибок нет, парсим JSON-ответ.
    QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObject = json.object(); // Преобразуем QJsonDocument в QJsonObject.

    // Дополнительная проверка на наличие ключевых полей в JSON-ответе.
    // Это помогает обработать случаи, когда API вернул 200 OK, но данные некорректны.
    if (!jsonObject.contains("main") || !jsonObject.contains("wind") || !jsonObject.contains("weather")) {
        qWarning() << "Invalid JSON response for weather data: Missing essential fields.";
        if (!weatherLabels.empty()) {
            weatherLabels[1]->setText("Error: Malformed data received.");
        }
        reply->deleteLater();
        return;
    }

    QJsonObject main = jsonObject["main"].toObject();       // Основные данные (температура, давление, влажность).
    QJsonObject wind = jsonObject["wind"].toObject();       // Данные о ветре.
    QJsonArray weatherArr = jsonObject["weather"].toArray(); // Массив с описанием погоды.

    // Проверяем, что массив 'weather' не пуст.
    if (weatherArr.isEmpty()) {
        qWarning() << "Weather array is empty in response.";
        if (!weatherLabels.empty()) {
            weatherLabels[1]->setText("Error: No weather description available.");
        }
        reply->deleteLater();
        return;
    }

    // Получаем температуру и описание погоды.
    QString temperature = QString::number(main["temp"].toDouble(), 'f', 1);
    QString description = weatherArr[0].toObject()["description"].toString();

    // Получаем время обновления данных (Unix timestamp в UTC) и смещение часового пояса города в секундах.
    qint64 dt_utc = jsonObject["dt"].toInteger();
    qint64 timezone_offset_seconds = jsonObject["timezone"].toInteger();

    // Создаем QDateTime объект из UTC Unix timestamp.
    QDateTime updatedDateTimeUtc = QDateTime::fromSecsSinceEpoch(dt_utc, Qt::UTC);

    // Добавляем смещение часового пояса, чтобы получить местное время города.
    QDateTime localCityTime = updatedDateTimeUtc.addSecs(timezone_offset_seconds);

    // Обновляем текст в QLabel с полученными данными.
    weatherLabels[0]->setText(temperature + "°C");
    weatherLabels[1]->setText(description);
    weatherLabels[2]->setText(QString("Pressure: %1 hPa").arg(main["pressure"].toInt()));
    weatherLabels[3]->setText(QString("Humidity: %1%").arg(main["humidity"].toInt()));
    weatherLabels[4]->setText(QString("Wind Speed: %1 m/s").arg(wind["speed"].toDouble()));
    weatherLabels[5]->setText(QString("Updated at: %1").arg(localCityTime.toString("hh:mm")));

    // Определяем путь к иконке погоды и обновляем QLabel с иконкой.
    QString iconPath = getIconForWeather(description);

    qreal screenPixelRatio = 1.0;
    if (QGuiApplication::primaryScreen()) {
        screenPixelRatio = QGuiApplication::primaryScreen()->devicePixelRatio();
    }
    weatherIcon->setPixmap(renderSvg(iconPath, QSize(40, 40), screenPixelRatio));

    reply->deleteLater(); // Удаляем объект QNetworkReply.
}

// Вспомогательная функция для сопоставления описания погоды с SVG-иконками.
QString CenterLayout::getIconForWeather(const QString& description)
{
    QString desc = description.toLower(); // Приводим описание к нижнему регистру для сравнения.
    if (desc.contains("snow")) return ":/icons/snowflake.svg";
    if (desc.contains("rain") || desc.contains("drizzle")) return ":/icons/cloud-rain.svg"; // Добавил "drizzle".
    if (desc.contains("storm") || desc.contains("thunder")) return ":/icons/cloud-lightning.svg";
    if (desc.contains("clear")) return ":/icons/sun.svg";
    if (desc.contains("cloud") || desc.contains("overcast") || desc.contains("scattered clouds")) return ":/icons/cloud.svg"; // Добавил "overcast" и "scattered clouds".
    if (desc.contains("fog") || desc.contains("mist") || desc.contains("haze") || desc.contains("smoke") || desc.contains("dust")) return ":/icons/cloud-fog.svg"; // Добавил больше туманных условий.
    if (desc.contains("squalls") || desc.contains("tornado")) return ":/icons/wind.svg"; // Или что-то более подходящее для сильного ветра/шторма.
    return ":/icons/thermometer-simple.svg"; // Возвращаем запасную иконку, если совпадений не найдено.
}
