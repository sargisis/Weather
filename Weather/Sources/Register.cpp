#include "../Headers/Register.h" // Включаем заголовочный файл для нашего класса RegisterWindow
#include <QDebug> // Для вывода отладочных сообщений (например, qWarning)
#include <QGuiApplication> // Для доступа к QApplication (для центрирования окна)
#include <QScreen> // Для получения информации о экране (для центрирования окна)

// Конструктор класса RegisterWindow.
// parent - родительский виджет.
// manager (std::make_unique<QNetworkAccessManager>(this)) - инициализирует уникальный указатель
// на QNetworkAccessManager, который будет управлять сетевыми запросами. 'this' указывается
// как родитель, чтобы менеджер автоматически удалился при уничтожении RegisterWindow.
RegisterWindow::RegisterWindow(QWidget *parent)
    : QWidget(parent), manager(std::make_unique<QNetworkAccessManager>(this))
{
    // Настройка основного окна регистрации.
    setWindowTitle("Create an Account"); // Устанавливаем заголовок окна.
    setFixedSize(360, 300);             // Устанавливаем фиксированный размер окна.
    setStyleSheet("background-color: #1e1e1e; color: white;"); // Применяем общие стили через QSS.

    // Определение стилей для полей ввода.
    QString inputStyle = R"(
        QLineEdit {
            padding: 10px;            /* Внутренние отступы поля ввода */
            font-size: 14px;          /* Размер шрифта */
            border-radius: 8px;       /* Скругление углов */
            background: #2d2d2d;      /* Темно-серый фон */
            color: white;             /* Белый цвет текста */
            border: 1px solid #555;   /* Серая рамка */
        }
        QLineEdit:focus { /* Стиль при фокусе на поле ввода */
            border: 1px solid #0078D7; /* Синяя рамка */
        }
    )";

    // Определение стилей для кнопок.
    QString btnStyle = R"(
        QPushButton {
            background-color: #0078D7; /* Синий фон кнопки */
            color: white;             /* Белый цвет текста */
            border-radius: 8px;       /* Скругление углов */
            padding: 10px;
            font-size: 14px;
            font-weight: bold;        /* Жирный шрифт */
        }
        QPushButton:hover { /* Стиль при наведении курсора */
            background-color: #005fa3;
        }
    )";

    // Создание заголовка окна.
    auto* title = new QLabel("Create Account");
    title->setStyleSheet("font-size: 22px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter); // Выравнивание по центру.

    // Создание полей ввода и применение стилей.
    email_input = std::make_unique<QLineEdit>();
    email_input->setPlaceholderText("Email");
    email_input->setStyleSheet(inputStyle);

    password_input = std::make_unique<QLineEdit>();
    password_input->setPlaceholderText("Password");
    password_input->setEchoMode(QLineEdit::Password); // Скрывает вводимый текст (для паролей).
    password_input->setStyleSheet(inputStyle);

    confirm_input = std::make_unique<QLineEdit>();
    confirm_input->setPlaceholderText("Confirm Password");
    confirm_input->setEchoMode(QLineEdit::Password); // Скрывает вводимый текст.
    confirm_input->setStyleSheet(inputStyle);

    // Создание кнопки регистрации и применение стилей.
    registerBtn = std::make_unique<QPushButton>("Register");
    registerBtn->setStyleSheet(btnStyle);
    registerBtn->setMinimumHeight(40); // Установка минимальной высоты кнопки.

    // Создание основного вертикального макета и добавление виджетов.
    auto* layout = new QVBoxLayout;
    layout->addWidget(title);             // Добавляем заголовок.
    layout->addSpacing(10);               // Добавляем вертикальный отступ.
    layout->addWidget(email_input.get()); // Добавляем поле email.
    layout->addWidget(password_input.get()); // Добавляем поле пароля.
    layout->addWidget(confirm_input.get()); // Добавляем поле подтверждения пароля.
    layout->addSpacing(15);
    layout->addWidget(registerBtn.get()); // Добавляем кнопку регистрации.
    layout->addStretch();                 // Добавляет растягивающееся пространство, чтобы элементы прижимались к верху.
    layout->setContentsMargins(30, 20, 30, 20); // Устанавливаем отступы от краев окна.
    setLayout(layout);                    // Устанавливаем этот макет для окна.

    // Центрирование окна на экране (добавлено для полноты, если это окно не центрируется родительским).
    // QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    // int x = (screenGeometry.width() - width()) / 2;
    // int y = (screenGeometry.height() - height()) / 2;
    // move(x, y);

    // Установка соединения сигнала и слота для кнопки регистрации.
    // При нажатии на кнопку registerBtn, будет вызвана лямбда-функция.
    connect(registerBtn.get(), &QPushButton::clicked, this, [this]() {
        QString email = email_input->text().trimmed();     // Получаем email, удаляя пробелы.
        QString password = password_input->text();         // Получаем пароль.
        QString confirm = confirm_input->text();           // Получаем подтверждение пароля.

        // Проверка на заполнение всех полей.
        if (email.isEmpty() || password.isEmpty() || confirm.isEmpty()) {
            QMessageBox::warning(this, "Error", "All fields are required.");
            return; // Прерываем выполнение, если какое-либо поле пустое.
        }

        // Проверка совпадения паролей.
        if (password != confirm) {
            QMessageBox::warning(this, "Error", "Passwords do not match.");
            return; // Прерываем выполнение, если пароли не совпадают.
        }

        // Если все проверки пройдены, вызываем метод для регистрации пользователя.
        RegisterUser(email, password);
    });
}

// Метод для регистрации нового пользователя с использованием Firebase Authentication.
void RegisterWindow::RegisterUser(const QString &email, const QString &password) {
    // Ваш Firebase Web API Key.
    const QString API_KEY = "AIzaSyBwbdcguYkqe8t4mtYJ8QRQgv_V6TtSL4A";

    // Формируем URL для Firebase Authentication REST API для регистрации.
    QUrl url("https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + API_KEY);

    QNetworkRequest request(url); // Создаем объект сетевого запроса.
    // Устанавливаем заголовок Content-Type, указывая, что тело запроса будет в формате JSON.
    request.setHeader(QNetworkRequest::ContentTypeHeader , "application/json");

    QJsonObject json; // Создаем JSON-объект для тела POST-запроса.
    json["email"] = email;     // Добавляем email пользователя.
    json["password"] = password; // Добавляем пароль пользователя.
    // Запрашиваем "secure token" (ID токен Firebase) после успешной регистрации.
    json["returnSecureToken"] = true;

    // Проверяем, что manager инициализирован.
    // Хотя в конструкторе он уже инициализируется, эта проверка может быть полезна,
    // если manager мог быть сброшен или переинициализирован в другом месте.
    if (!manager){
        manager = std::make_unique<QNetworkAccessManager>(this);
    }
    // Подключаем сигнал finished от QNetworkAccessManager к лямбда-функции.
    // Эта функция будет выполнена, когда запрос завершится.
    connect(manager.get() , &QNetworkAccessManager::finished , this , [this, email, password](QNetworkReply* reply){
        // Проверяем, не было ли сетевых ошибок.
        if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray array = reply->readAll(); // Читаем данные ответа.
            qDebug() << "Registration Successful" << array; // Выводим ответ в отладочную консоль.

            QMessageBox::information(this, "Success", "Account created successfully!"); // Показываем сообщение об успехе.

            // Испускаем сигнал об успешной регистрации, передавая email и пароль.
            // Это может быть использовано для автоматического входа после регистрации.
            emit registrationSuccess(email , password);
            this->close(); // Закрываем окно регистрации.
        }
        else {
            // Если произошла ошибка, выводим ее в отладочную консоль.
            qDebug() << "Registration Failed" << reply->errorString();
            // Firebase возвращает более детальные ошибки в теле JSON, например, "EMAIL_EXISTS".
            // Для более информативных сообщений пользователю, нужно парсить JSON-ответ.
            QMessageBox::warning(this, "Registration Error", "Failed to create account: " + reply->errorString());
        }
        reply->deleteLater(); // Помечаем объект QNetworkReply для удаления.
    });

    // Отправляем POST-запрос с JSON-данными.
    manager->post(request , QJsonDocument(json).toJson());
}
