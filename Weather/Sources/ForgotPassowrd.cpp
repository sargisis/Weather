#include "../Headers/ForgotPassowrd.h"
#include <QDebug>           // Для вывода отладочных сообщений
#include <QHBoxLayout>      // Для горизонтального расположения виджетов (например, иконки и текста заголовка)
#include <QSvgRenderer>     // Для рендеринга SVG-изображений
#include <QPainter>         // Для рисования (используется QSvgRenderer для отрисовки на QPixmap)
#include <QIcon>            // Для установки иконки окна

// Вспомогательная функция для рендеринга SVG-изображений в QPixmap.
// Это полезно для отображения SVG, так как QPixmap может быть использован напрямую
// в QLabel, QIcon и других виджетах.
QPixmap renderSvgForForgotPassword(const QString& path, const QSize& baseSize, qreal devicePixelRatio)
{
    // Вычисляем фактический размер пиксмапа с учетом плотности пикселей устройства (для HiDPI дисплеев).
    QSize actualSize = baseSize * devicePixelRatio;
    QPixmap pixmap(actualSize);
    pixmap.fill(Qt::transparent); // Делаем фон прозрачным.

    QSvgRenderer renderer(path); // Создаем рендерер для SVG из указанного пути.
    if (!renderer.isValid()) {   // Проверяем, удалось ли загрузить SVG.
        qWarning() << "Failed to load SVG:" << path; // Выводим предупреждение, если не удалось.
        return QPixmap(); // Возвращаем пустой QPixmap.
    }

    QPainter painter(&pixmap); // Создаем объект QPainter для рисования на pixmap.
    renderer.render(&painter); // Рендерим SVG на pixmap.

    pixmap.setDevicePixelRatio(devicePixelRatio); // Устанавливаем плотность пикселей для корректного отображения.
    return pixmap; // Возвращаем готовый QPixmap.
}


ForgotPasswordWindow::ForgotPasswordWindow(QWidget *parent)
    : QWidget(parent), manager(new QNetworkAccessManager(this)) // Инициализация базового класса и QNetworkAccessManager
{
    // --- Изменения для иконки окна начинаются здесь ---
    // Устанавливаем заголовок окна. Ранее здесь был эмодзи, теперь только текст.
    setWindowTitle("Reset Password");

    // Создаем объект QIcon для установки иконки в заголовке окна и на панели задач.
    QIcon windowIcon;
    // Определяем коэффициент плотности пикселей экрана для корректного отображения иконки на HiDPI дисплеях.
    qreal devicePixelRatio = QGuiApplication::primaryScreen() ? QGuiApplication::primaryScreen()->devicePixelRatio() : 1.0;
    // Загружаем SVG-иконку 'key.svg' из ресурсов (путь ":/icons/key.svg")
    // и добавляем ее в QIcon в размере 24x24 (это базовый размер, фактический может быть больше на HiDPI).
    windowIcon.addPixmap(renderSvgForForgotPassword(":/icons/key.svg", QSize(24, 24), devicePixelRatio));
    // Устанавливаем созданный QIcon как иконку для текущего окна.
    setWindowIcon(windowIcon);
    // --- Изменения для иконки окна заканчиваются здесь ---

    // Установка фиксированного размера окна.
    setFixedSize(380, 200);
    // Применение стилей CSS для виджетов в окне.
    setStyleSheet(R"(
        QWidget {
            background-color: #1e1e1e; /* Цвет фона окна */
            color: white;             /* Цвет текста */
            border-radius: 12px;      /* Радиус скругления углов окна */
        }
        QLineEdit {
            padding: 10px;            /* Отступы внутри поля ввода */
            font-size: 14px;          /* Размер шрифта */
            border-radius: 8px;       /* Радиус скругления углов поля ввода */
            background: #2c2c2c;      /* Цвет фона поля ввода */
            color: white;             /* Цвет текста в поле ввода */
            border: 1px solid #555;   /* Граница поля ввода */
        }
        QPushButton {
            background-color: #0078D7; /* Цвет фона кнопки */
            color: white;             /* Цвет текста на кнопке */
            border-radius: 8px;       /* Радиус скругления углов кнопки */
            padding: 10px;
            font-size: 14px;
            font-weight: bold;        /* Жирный шрифт */
        }
        QPushButton:hover {
            background-color: #005fa3; /* Цвет фона кнопки при наведении */
        }
    )");

    // Этот блок кода добавляет иконку *внутри* окна, рядом с текстовым заголовком.
    // Если вы хотите иметь и системную иконку (установленную выше), и иконку внутри окна, оставьте этот блок.
    // Если вам нужна только системная иконка, этот блок можно удалить.

    // Создаем QLabel для отображения иконки внутри окна.
    QLabel* iconLabel = new QLabel(this);
    // Загружаем и рендерим SVG-иконку 'key.svg' для отображения внутри UI.
    // Используем тот же renderSvgForForgotPassword и devicePixelRatio, который был объявлен выше.
    iconLabel->setPixmap(renderSvgForForgotPassword(":/icons/key.svg", QSize(24, 24), devicePixelRatio));
    iconLabel->setFixedSize(24, 24);    // Устанавливаем фиксированный размер для QLabel с иконкой.
    iconLabel->setScaledContents(true); // Масштабируем содержимое QLabel для заполнения всего его размера.

    // Создаем QLabel для текстового заголовка внутри окна.
    QLabel* titleTextLabel = new QLabel("Enter your email to reset password");
    titleTextLabel->setStyleSheet("font-size: 16px; font-weight: bold;"); // Применяем стили к тексту.
    titleTextLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);      // Выравниваем текст по левому краю и по центру по вертикали.

    // Создаем горизонтальный макет для объединения иконки и текстового заголовка.
    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setAlignment(Qt::AlignCenter); // Центрируем весь горизонтальный макет.
    titleLayout->addWidget(iconLabel);           // Добавляем иконку.
    titleLayout->addSpacing(8);                  // Добавляем небольшой отступ между иконкой и текстом.
    titleLayout->addWidget(titleTextLabel);      // Добавляем текстовый заголовок.


    // Создаем поле ввода для email.
    email_input = new QLineEdit;
    email_input->setPlaceholderText("Your email address"); // Текст-заполнитель.

    // Создаем кнопку отправки.
    sendBtn = new QPushButton("Send Reset Link");

    // Создаем основной вертикальный макет для окна.
    auto* layout = new QVBoxLayout;
    layout->addLayout(titleLayout);     // Добавляем горизонтальный макет с иконкой и заголовком.
    layout->addSpacing(10);             // Добавляем отступ.
    layout->addWidget(email_input);     // Добавляем поле ввода email.
    layout->addSpacing(10);
    layout->addWidget(sendBtn);         // Добавляем кнопку отправки.
    layout->addStretch();               // Добавляет "растяжку", чтобы элементы прижимались к верхней части.
    layout->setContentsMargins(20, 20, 20, 20); // Устанавливаем отступы от краев окна.
    setLayout(layout);                  // Устанавливаем этот макет для окна.

    // Центрирование окна на экране.
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry(); // Получаем геометрию основного экрана.
    int x = (screenGeometry.width() - width()) / 2;    // Вычисляем X-координату для центрирования.
    int y = (screenGeometry.height() - height()) / 2; // Вычисляем Y-координату для центрирования.
    move(x, y); // Перемещаем окно.

    // Соединение сигнала clicked от кнопки sendBtn со слотом onSendClicked.
    connect(sendBtn, &QPushButton::clicked, this, &ForgotPasswordWindow::onSendClicked);
}

// Слот, вызываемый при нажатии на кнопку "Send Reset Link".
void ForgotPasswordWindow::onSendClicked() {
    QString email = email_input->text().trimmed(); // Получаем текст из поля email, удаляя пробелы по краям.
    if (!email.isEmpty()) // Проверяем, что email не пуст.
    {
        onForgotPasswordImpl(email); // Если email введен, вызываем функцию для отправки запроса.
    }
    else // Если email пуст, выводим предупреждение.
    {
        QMessageBox::warning(this, "Input Error", "Please enter your email.");
    }
}

// Отправляет запрос на сброс пароля к сервису Firebase Authentication.
void ForgotPasswordWindow::onForgotPasswordImpl(const QString &email)
{
    // Ваш API-ключ для Firebase. В реальных проектах он должен быть частью конфигурации,
    // а не жестко закодирован, но для клиента это обычно не секрет.
    const QString API_KEY = "AIzaSyC6Jtos6Qydb0SuwCo8AFUSwyNJ-H7Vkys";

    // Формируем URL для Firebase Authentication API для отправки кода подтверждения сброса пароля.
    QUrl url("https://identitytoolkit.googleapis.com/v1/accounts:sendOobCode?key=" + API_KEY);

    QNetworkRequest request(url); // Создаем объект сетевого запроса.

    // Устанавливаем заголовок "Content-Type" как "application/json",
    // так как мы отправляем данные в формате JSON.
    request.setHeader(QNetworkRequest::ContentTypeHeader , "application/json");

    QJsonObject json; // Создаем JSON-объект для тела POST-запроса.
    json["requestType"] = "PASSWORD_RESET"; // Тип запроса: сброс пароля.
    json["email"] = email;                  // Email пользователя, для которого сбрасывается пароль.

    // Отправляем POST-запрос с JSON-данными.
    QNetworkReply* reply = manager->post(request , QJsonDocument(json).toJson());
    // Подключаем сигнал finished от QNetworkReply к лямбда-функции.
    // Эта функция будет выполнена, когда запрос завершится (успешно или с ошибкой).
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QByteArray response = reply->readAll(); // Читаем все данные из ответа.
        qDebug() << "Firebase response:" << response; // Выводим ответ сервера в консоль для отладки.

        if (reply->error() == QNetworkReply::NoError) { // Проверяем, не было ли сетевых ошибок.
            // Примечание: Firebase может вернуть 200 OK, даже если email не зарегистрирован.
            // Для более точной обработки ошибок Firebase нужно парсить тело JSON-ответа,
            // где будут указаны конкретные коды ошибок (например, "EMAIL_NOT_FOUND").
            // Здесь для простоты предполагается, что отсутствие сетевой ошибки означает успех.
            QMessageBox::information(this, "Success", "If an account with this email exists, a reset link has been sent.");
            this->close(); // Закрываем окно после успешной отправки.
        } else { // Если произошла сетевая ошибка (например, нет интернет-соединения).
            QMessageBox::warning(this, "Error", reply->errorString()); // Выводим сообщение об ошибке.
        }

        reply->deleteLater(); // Помечаем объект QNetworkReply для удаления.
            // Это важно для предотвращения утечек памяти.
    });
}
