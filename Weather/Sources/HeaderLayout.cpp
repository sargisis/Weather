#include "../Headers/HeaderLayout.h" // Включаем заголовочный файл для класса HeaderLayout
#include <QAction>           // Для добавления иконок в QLineEdit
#include <QIcon>             // Для работы с иконками
#include <QJsonDocument>     // Для парсинга JSON-ответов
#include <QJsonObject>       // Для работы с JSON-объектами
#include <QJsonArray>        // Для работы с JSON-массивами
#include <QUrlQuery>         // Для формирования параметров URL-запроса
#include <QUrl>              // Для работы с URL-адресами
#include <QDebug>            // Для вывода отладочных сообщений

// Конструктор класса HeaderLayout.
// Наследуется от QHBoxLayout, поэтому может быть использован как макет.
HeaderLayout::HeaderLayout(QWidget* parent)
    : QHBoxLayout(parent), // Вызов конструктора базового класса QHBoxLayout
    m_search(std::make_unique<QLineEdit>()) // Инициализация unique_ptr для QLineEdit
{
    // Применение CSS-стилей к полю ввода m_search.
    m_search->setStyleSheet(R"(
        QLineEdit {
            padding: 8px 36px 8px 12px; /* Внутренние отступы (top right bottom left) */
            font-size: 16px;          /* Размер шрифта */
            border-radius: 12px;      /* Скругление углов */
            background-color: #2d2d2d; /* Темный фон поля */
            border: 1px solid #444;   /* Серая граница */
            color: white;             /* Цвет текста */
        }
        QLineEdit:focus { /* Стиль при фокусе на поле ввода */
            border: 1px solid #007BFF; /* Синяя граница */
        }
    )");

    // Установка текста-заполнителя для поля ввода.
    m_search->setPlaceholderText("Search your city...");

    // Создание QAction с иконкой поиска и добавление его в QLineEdit.
    // QAction позволяет добавить кнопку или иконку прямо в поле ввода.
    // ":/icons/search.png" - путь к иконке в ресурсах Qt.
    QAction* searchIcon = new QAction(QIcon(":/icons/search.png"), "", m_search.get());
    m_search->addAction(searchIcon, QLineEdit::LeadingPosition); // Добавляем иконку в начальную позицию (слева).

    // Настройка макета.
    addStretch(); // Добавляем растягивающееся пространство слева (для центрирования поля ввода).
    addWidget(m_search.get(), 1); // Добавляем поле ввода, с фактором растяжения 1.
    addStretch(); // Добавляем растягивающееся пространство справа.

    // Устанавливаем отступы для всего макета.
    setContentsMargins(10, 10, 10, 10);

    // --- Инициализация автодополнения (Autocomplete) ---
    // Создаем QNetworkAccessManager для выполнения сетевых запросов к API автодополнения.
    m_autocompleteNetworkManager = new QNetworkAccessManager(this);
    // Создаем QStringListModel, который будет хранить список предложений для автодополнения.
    m_completerModel = new QStringListModel(this);
    // Создаем QCompleter и связываем его с моделью данных.
    m_cityCompleter = new QCompleter(m_completerModel, this);
    m_cityCompleter->setCaseSensitivity(Qt::CaseInsensitive); // Делаем автодополнение нечувствительным к регистру.
    m_cityCompleter->setFilterMode(Qt::MatchContains);       // Совпадение, если текст содержится где-либо в предложении.
    m_cityCompleter->setCompletionMode(QCompleter::PopupCompletion); // Предложения будут отображаться во всплывающем окне.
    // Устанавливаем QCompleter для поля ввода поиска.
    m_search->setCompleter(m_cityCompleter);

    // --- Подключение сигналов/слотов ---
    // Сигнал textEdited испускается, когда текст в поле ввода был изменен пользователем.
    // Подключаем его к нашему слоту onSearchTextEdited для запуска запросов автодополнения.
    connect(m_search.get(), &QLineEdit::textEdited, this, &HeaderLayout::onSearchTextEdited);
    // Сигнал finished от QNetworkAccessManager испускается после получения ответа на сетевой запрос.
    // Подключаем его к нашему слоту onAutocompleteDataReceived для обработки данных автодополнения.
    connect(m_autocompleteNetworkManager, &QNetworkAccessManager::finished, this, &HeaderLayout::onAutocompleteDataReceived);
    // Сигнал activated от QCompleter испускается, когда пользователь выбирает элемент из списка автодополнения.
    // Используем QOverload, чтобы явно указать версию activated(const QString&).
    connect(m_cityCompleter, QOverload<const QString&>::of(&QCompleter::activated), this, &HeaderLayout::onCityCompletionSelected);
    // Сигнал returnPressed испускается, когда пользователь нажимает Enter в поле ввода.
    // Подключаем его к нашему слоту onSearchReturnPressed для обработки введенного города.
    connect(m_search.get(), &QLineEdit::returnPressed, this, &HeaderLayout::onSearchReturnPressed);
}

// Метод для обновления текста-заполнителя в поле поиска.
// Используется для адаптации подсказки в зависимости от того, разрешен ли поиск по городу
// или только по стране (если это приложение для погоды, например).
void HeaderLayout::updateSearchPlaceholder(const QString& country, bool isCityAllowed)
{
    if (isCityAllowed) {
        m_search->setPlaceholderText("Search your city..."); // Стандартная подсказка для поиска по городу.
    } else {
        // Подсказка, указывающая, что поиск ограничен определенной страной.
        m_search->setPlaceholderText(QString("Search in %1...").arg(country));
    }
}

// Слот, вызываемый при изменении текста в поле поиска.
void HeaderLayout::onSearchTextEdited(const QString& text)
{
    // Если длина текста меньше 3 символов, очищаем список предложений.
    // Это предотвращает отправку слишком частых или неинформативных запросов.
    if (text.length() < 3) {
        m_completerModel->setStringList(QStringList());
        return;
    }

    // Формируем URL для Google Places Autocomplete API.
    QUrl url("https://maps.googleapis.com/maps/api/place/autocomplete/json");
    QUrlQuery query; // Объект для формирования параметров запроса.
    query.addQueryItem("input", text); // Входной текст для автодополнения.
    query.addQueryItem("types", "(cities)"); // Ограничиваем поиск только городами.
    query.addQueryItem("language", "en"); // Устанавливаем язык для предложений (английский).
    query.addQueryItem("key", GOOGLE_PLACES_API_KEY); // Используем ваш API-ключ Google Places.
    url.setQuery(query); // Устанавливаем параметры запроса для URL.

    // Отправляем GET-запрос к API автодополнения.
    m_autocompleteNetworkManager->get(QNetworkRequest(url));
}

// Слот, вызываемый после получения ответа от API автодополнения.
void HeaderLayout::onAutocompleteDataReceived(QNetworkReply* reply)
{
    // Проверяем наличие сетевых ошибок.
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Autocomplete API Error:" << reply->errorString(); // Выводим ошибку.
        reply->deleteLater(); // Помечаем объект QNetworkReply для удаления.
        return;
    }

    // Парсим JSON-ответ от API.
    QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObject = jsonResponse.object();

    QStringList suggestions; // Список для хранения полученных предложений.
    // Извлекаем массив "predictions" из JSON-объекта.
    QJsonArray predictions = jsonObject["predictions"].toArray();
    // Итерируемся по каждому предсказанию (предложению) в массиве.
    for (const QJsonValue& value : predictions) {
        QJsonObject prediction = value.toObject();
        // Извлекаем "description" (полное название города с регионом/страной) и добавляем в список.
        suggestions.append(prediction["description"].toString());
    }

    // Обновляем модель QCompleter новым списком предложений.
    m_completerModel->setStringList(suggestions);
    reply->deleteLater(); // Помечаем объект QNetworkReply для удаления.
}

// Слот, вызываемый, когда пользователь выбирает город из списка автодополнения.
void HeaderLayout::onCityCompletionSelected(const QString& text)
{
    // Испускаем сигнал citySelected, передавая только название города
    // (отделяем его от остальной части строки, например, "Лондон, Великобритания" -> "Лондон").
    emit citySelected(text.split(",").first().trimmed());
}

// Слот, вызываемый, когда пользователь нажимает Enter в поле поиска.
void HeaderLayout::onSearchReturnPressed()
{
    QString enteredText = m_search->text().trimmed(); // Получаем введенный текст.
    if (!enteredText.isEmpty()) { // Если текст не пуст.
        // Испускаем сигнал citySelected, аналогично выбору из автодополнения.
        emit citySelected(enteredText.split(",").first().trimmed());
    }
}
