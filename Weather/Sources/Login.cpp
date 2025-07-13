#include "../Headers/Login.h" // Включаем заголовочный файл для нашего класса LogINWindow

// Конструктор класса LogINWindow.
// parent - родительский виджет.
// manager (std::make_unique<QNetworkAccessManager>(this)) - инициализирует уникальный указатель
// на QNetworkAccessManager, который будет управлять сетевыми запросами. 'this' указывается
// как родитель, чтобы менеджер автоматически удалился при уничтожении LogINWindow.
LogINWindow::LogINWindow(QWidget* parent)
    : QWidget(parent)
    , manager (std::make_unique<QNetworkAccessManager>(this))
{
    // Устанавливаем заголовок окна.
    this->setWindowTitle("Logging in...");
    // Устанавливаем фиксированный размер окна.
    this->resize(300, 150);

    // Создаем вертикальный макет для размещения виджетов внутри окна.
    auto* layout = new QVBoxLayout(this);
    // Создаем метку для отображения сообщения "Please wait...".
    auto* label = new QLabel("Please wait...");
    // Применяем стили CSS к метке.
    label->setStyleSheet("color: white; font-size: 14px;");
    // Добавляем метку в макет.
    layout->addWidget(label);
    // Устанавливаем созданный макет для окна.
    this->setLayout(layout);

    // Применяем стили CSS к самому окну.
    this->setStyleSheet("background-color: #2b2b2b;");
}

// Метод для выполнения входа пользователя с использованием Firebase Authentication.
// email - электронная почта пользователя.
// password - пароль пользователя.
void LogINWindow::LogInUser(const QString &email, const QString &password)
{
    // Ваш Firebase Web API Key. Важно: для клиентских приложений это обычно не считается секретом.
    const QString API_KEY = "AIzaSyBwbdcguYkqe8t4mtYJ8QRQgv_V6TtSL4A";

    // Формируем URL для Firebase Authentication REST API для входа по паролю.
    QUrl url("https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + API_KEY);

    QNetworkRequest request(url); // Создаем объект сетевого запроса.
    // Устанавливаем заголовок Content-Type, указывая, что тело запроса будет в формате JSON.
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject object; // Создаем JSON-объект для тела POST-запроса.
    object["email"] = email;     // Добавляем email пользователя.
    object["password"] = password; // Добавляем пароль пользователя.
    // Запрашиваем "secure token" (ID токен Firebase), который нужен для аутентифицированных запросов.
    object["returnSecureToken"] = true;

    // Отправляем POST-запрос с JSON-данными.
    // Результат запроса (QNetworkReply) сохраняется в члене класса `reply`.
    reply = manager->post(request , QJsonDocument(object).toJson());
    // Подключаем сигнал finished от QNetworkAccessManager к лямбда-функции.
    // Эта лямбда-функция будет выполнена, когда запрос завершится.
    // Внимание: Подключение к manager.get()::finished вместо reply::finished
    // означает, что эта лямбда будет вызвана для *любого* завершенного запроса,
    // отправленного через этот `manager`. В данном случае, это может быть не критично,
    // так как обычно `LogINWindow` делает только один такой запрос, но в более
    // сложных сценариях лучше подключаться к `reply->finished()`.
    QObject::connect(manager.get(), &QNetworkAccessManager::finished, this, [this]() {
        // Проверяем, не было ли сетевых ошибок во время запроса.
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray array = reply->readAll(); // Читаем все данные из ответа сервера.
            QJsonDocument doc = QJsonDocument::fromJson(array); // Парсим ответ как JSON-документ.
            // Извлекаем idToken и email из JSON-ответа.
            QString idToken = doc.object()["idToken"].toString();
            QString email = doc.object()["email"].toString();

            // Создаем и настраиваем QMessageBox для сообщения об успешном входе.
            auto* msgBox = new QMessageBox(this);
            msgBox->setWindowTitle("LogIn Successful");
            msgBox->setText("Welcome: " + email); // Отображаем приветственное сообщение.
            msgBox->setIcon(QMessageBox::Information); // Устанавливаем иконку информационного сообщения.
            // Применяем стили CSS к QMessageBox.
            msgBox->setStyleSheet("QMessageBox { background-color: #2b2b2b; color: white; }");

            // Центрируем QMessageBox на экране.
            msgBox->adjustSize(); // Подстраиваем размер QMessageBox под содержимое.
            QRect screenGeometry = QGuiApplication::primaryScreen()->geometry(); // Получаем геометрию основного экрана.
            int x = (screenGeometry.width() - msgBox->width()) / 2;    // Вычисляем X-координату.
            int y = (screenGeometry.height() - msgBox->height()) / 2; // Вычисляем Y-координату.
            msgBox->move(x, y); // Перемещаем QMessageBox.

            msgBox->exec(); // Отображаем модальное окно сообщения и ждем закрытия.

            loginSuccessful = true; // Устанавливаем флаг успешного входа.
            this->close(); // Закрываем окно LogINWindow.
        } else {
            // Если произошла сетевая ошибка, отображаем предупреждение.
            // reply->errorString() предоставляет текстовое описание ошибки.
            QMessageBox::warning(this , "Error", "Please check your email or password:\n" + reply->errorString());
        }
        // Важно: помечаем объект QNetworkReply для удаления после обработки.
        // Это предотвращает утечки памяти.
        reply->deleteLater();
    });
}
