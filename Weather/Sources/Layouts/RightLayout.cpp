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

// Реализация функции createLayouts для настройки внешнего вида макета
void RightLayout::createLayouts()
{
    setSpacing(15); // Устанавливаем отступ между элементами в вертикальном макете.
    setContentsMargins(10, 10, 10, 10); // Устанавливаем отступы от краев макета.
    setAlignment(Qt::AlignTop); // Выравниваем содержимое по верхнему краю.
}

// Функция для запроса данных о будущем прогнозе погоды для указанного города.
void RightLayout::fetchFutureWeather(const QString& city)
{
    clearCards(); // Перед новым запросом очищаем все существующие карточки прогноза.
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
        QString errorMessage;
        int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        // Если статус-код 404 (Not Found), значит, город не найден.
        if (httpStatus == 404) {
            errorMessage = "City not found. Please enter a valid city name.";
            qWarning() << "City not found for forecast data. HTTP 404."; // Логируем предупреждение.
        } else {
            // Для всех других сетевых ошибок.
            errorMessage = "Failed to load forecast data: " + reply->errorString();
            qWarning() << "Network error for forecast data:" << reply->errorString(); // Логируем предупреждение.
        }

        clearCards(); // Очищаем старые карточки, чтобы показать только ошибку.
        auto* errorCard = createForecastCard("Error", 0, errorMessage); // Создаем специальную карточку для ошибки.
        this->addWidget(errorCard); // Добавляем карточку ошибки в макет.
        forecastCards.push_back(errorCard); // Добавляем в список для очистки при следующем запросе.

        reply->deleteLater(); // Удаляем объект QNetworkReply.
        return; // Выходим из функции, так как произошла ошибка.
    }

    // Если ошибок нет, парсим JSON-ответ.
    QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObject = json.object(); // Преобразуем QJsonDocument в QJsonObject.

    // Проверка наличия ключевых полей в JSON-ответе.
    if (!jsonObject.contains("list")) {
        qWarning() << "Invalid JSON response for forecast data: Missing 'list' field.";
        clearCards();
        auto* errorCard = createForecastCard("Error", 0, "Error: Malformed forecast data.");
        this->addWidget(errorCard);
        forecastCards.push_back(errorCard);
        reply->deleteLater();
        return;
    }

    QJsonArray list = jsonObject["list"].toArray(); // Получаем массив прогнозов.

    clearCards(); // Очищаем старые карточки перед добавлением новых данных.
    QSet<QString> uniqueDays; // Используем QSet для отслеживания уникальных дней.

    // Проходим по каждому элементу в массиве прогнозов.
    for (const auto& item : list) {
        auto entry = item.toObject();
        QString dt_txt = entry["dt_txt"].toString(); // Дата и время прогноза в строковом формате.
        QDateTime dateTime = QDateTime::fromString(dt_txt, "yyyy-MM-dd HH:mm:ss"); // Преобразуем строку в QDateTime.

        // Показываем только прогнозы на полдень (12:00) для каждого дня.
        // Это помогает избежать слишком большого количества карточек и обеспечивает прогноз на каждый день.
        if (dateTime.time().hour() != 12)
            continue;

        QString dayKey = dateTime.date().toString("yyyy-MM-dd"); // Ключ дня (например, "2025-07-04").
        // Если для этого дня уже есть прогноз, пропускаем.
        if (uniqueDays.contains(dayKey))
            continue;

        uniqueDays.insert(dayKey); // Добавляем день в набор уникальных дней.

        double temp = entry["main"].toObject()["temp"].toDouble(); // Температура.
        QString desc = entry["weather"].toArray()[0].toObject()["description"].toString(); // Описание погоды.
        QString timeStr = dateTime.toString("ddd dd MMM"); // Форматируем дату для отображения (например, "Fri 04 Jul").

        // Создаем новую карточку прогноза и добавляем ее в макет.
        auto* card = createForecastCard(timeStr, temp, desc);
        this->addWidget(card);
        forecastCards.push_back(card); // Добавляем карточку в вектор для дальнейшей очистки.

        // Ограничиваем количество отображаемых карточек (например, до 5 дней).
        if (forecastCards.size() >= 5)
            break;
    }

    reply->deleteLater(); // Удаляем объект QNetworkReply.
}

// Вспомогательная функция для создания одной карточки прогноза.
QFrame* RightLayout::createForecastCard(const QString& time, double temp, const QString& desc)
{
    auto* card = new QFrame; // Создаем фрейм для карточки.
    auto* cardLayout = new QHBoxLayout(card); // Основной горизонтальный макет внутри карточки.
    cardLayout->setContentsMargins(10, 10, 10, 10); // Отступы внутри карточки.
    cardLayout->setSpacing(10); // Пространство между элементами в макете.

    // Получаем текущий devicePixelRatio экрана для корректного рендеринга SVG.
    qreal screenPixelRatio = 1.0;
    if (QGuiApplication::primaryScreen()) {
        screenPixelRatio = QGuiApplication::primaryScreen()->devicePixelRatio();
    }

    // --- Иконка погоды (облако/солнце/снежинка и т.д.) ---
    QLabel* weatherIconLabel = new QLabel; // Метка для главной иконки погоды в карточке.
    weatherIconLabel->setFixedSize(30, 30); // Фиксированный размер для иконки.
    weatherIconLabel->setStyleSheet("background: transparent;"); // Прозрачный фон.
    QString iconPath = getIconForWeather(desc); // Получаем путь к иконке по описанию.
    weatherIconLabel->setPixmap(renderSvg(iconPath, QSize(30, 30), screenPixelRatio)); // Рендерим иконку.

    // --- Контейнер для текста и иконки температуры ---
    auto* textAndTempLayout = new QVBoxLayout(); // Вертикальный макет для текста (время, температура, описание).
    textAndTempLayout->setContentsMargins(0, 0, 0, 0); // Без отступов.
    textAndTempLayout->setSpacing(2); // Небольшой отступ между элементами.

    // 1. Label для времени/даты
    QLabel* timeLabel = new QLabel(QString("<b>%1</b>").arg(time)); // Метка для даты/времени (жирный текст).
    timeLabel->setStyleSheet("color: white; font-size: 16px; background: transparent;");
    timeLabel->setWordWrap(true); // Разрешить перенос слов.
    textAndTempLayout->addWidget(timeLabel); // Добавляем в вертикальный макет.

    // 2. Горизонтальный Layout для иконки температуры и самой температуры
    auto* tempRowLayout = new QHBoxLayout(); // Горизонтальный макет для иконки термометра и значения температуры.
    tempRowLayout->setContentsMargins(0, 0, 0, 0);
    tempRowLayout->setSpacing(5); // Отступ между иконкой и текстом температуры.

    QLabel* tempIconLabel = new QLabel; // Метка для иконки термометра.
    tempIconLabel->setFixedSize(20, 20); // Размер иконки термометра.
    tempIconLabel->setStyleSheet("background: transparent;");
    tempIconLabel->setPixmap(renderSvg(":/icons/thermometer-simple.svg", QSize(20, 20), screenPixelRatio)); // Рендерим иконку термометра.
    tempRowLayout->addWidget(tempIconLabel); // Добавляем иконку.

    QLabel* tempValueLabel = new QLabel(QString("%1 °C").arg(temp, 0, 'f', 1)); // Метка для значения температуры.
    tempValueLabel->setStyleSheet("color: white; font-size: 14px; background: transparent;");
    tempRowLayout->addWidget(tempValueLabel); // Добавляем значение температуры.
    tempRowLayout->addStretch(1); // Растягивание, чтобы прижать элементы влево.

    textAndTempLayout->addLayout(tempRowLayout); // Добавляем горизонтальный макет температуры в вертикальный.

    // 3. Label для описания погоды
    QLabel* descLabel = new QLabel(desc); // Метка для описания погоды.
    descLabel->setStyleSheet("color: white; font-size: 14px; background: transparent;");
    descLabel->setWordWrap(true); // Разрешить перенос слов.
    textAndTempLayout->addWidget(descLabel); // Добавляем в вертикальный макет.

    // Добавляем главную иконку погоды и новый вертикальный Layout с текстом
    // в основной горизонтальный Layout карточки.
    cardLayout->addWidget(weatherIconLabel);
    cardLayout->addLayout(textAndTempLayout, 1); // textAndTempLayout занимает оставшееся пространство.

    // Применяем CSS-стили к карточке.
    card->setStyleSheet(R"(
        QFrame {
            background-color: #0a84ff; /* Синий фон */
            color: white;
            padding: 0px;
            border-radius: 14px;
        }
    )");

    return card; // Возвращаем созданную карточку.
}

// Функция для очистки всех карточек прогноза из макета.
void RightLayout::clearCards()
{
    // Проходим по всем указателям на QFrame в векторе forecastCards.
    for (auto* card : forecastCards) {
        this->removeWidget(card); // Удаляем виджет из макета.
        delete card; // Удаляем сам виджет (освобождаем память).
    }
    forecastCards.clear(); // Очищаем вектор.
}


