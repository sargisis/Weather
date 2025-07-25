    #include "../Headers/Auth.h" // Включаем заголовочный файл для класса AuthWindow

    // Конструктор AuthWindow
    AuthWindow::AuthWindow(QWidget* parent)
        : QWidget(parent) // Вызываем конструктор базового класса QWidget
    {
        // Настройка основного окна аутентификации
        this->setFixedSize(380, 460); // Устанавливаем фиксированный размер окна.
        this->setWindowTitle("Weather Login"); // Устанавливаем заголовок окна.
        // Применяем общие стили для окна через QSS (Qt Style Sheets).
        this->setStyleSheet("background-color: #1e1e1e; color: white; border-radius: 12px;");

        // Определение стилей для различных элементов UI с помощью QSS
        QString inputStyle = R"(
            QLineEdit {
                padding: 10px;
                font-size: 14px;
                border-radius: 8px;
                background: #2c2c2c;
                color: white;
                border: 1px solid #444;
            }
            QLineEdit:focus { /* Стиль при фокусе на поле ввода */
                border: 1px solid #0078D7;
            }
        )";

        QString btnStyle = R"(
            QPushButton {
                background-color: #0078D7; /* Стандартный синий цвет */
                color: white;
                border-radius: 8px;
                padding: 8px;
                font-weight: bold;
                font-size: 14px;
            }
            QPushButton:hover { /* Стиль при наведении курсора */
                background-color: #005fa3;
            }
        )";

        QString googleBtnStyle = R"(
            QPushButton {
                background-color: #4285F4; /* Цвет Google */
                color: white;
                border-radius: 8px;
                padding: 8px;
                font-weight: bold;
                font-size: 14px;
            }
            QPushButton:hover {
                background-color: #3367D6;
            }
        )";

        // Создание элементов заголовка и подзаголовка
        auto* title = new QLabel("Welcome Back 👋"); // Заголовок окна.
        title->setStyleSheet("font-size: 24px; font-weight: bold;");
        title->setAlignment(Qt::AlignCenter); // Выравнивание по центру.

        auto* subtitle = new QLabel("Sign in to continue"); // Подзаголовок.
        subtitle->setStyleSheet("font-size: 14px; color: #aaaaaa;");
        subtitle->setAlignment(Qt::AlignCenter);

        // Создание полей ввода
        email_input = new QLineEdit;
        email_input->setPlaceholderText("Email"); // Текст-заполнитель.
        email_input->setStyleSheet(inputStyle); // Применяем определенный стиль.

        password_input = new QLineEdit;
        password_input->setPlaceholderText("Password");
        password_input->setEchoMode(QLineEdit::Password); // Скрывает вводимый текст (для паролей).
        password_input->setStyleSheet(inputStyle);

        // Создание кнопок
        auto* loginBtn = new QPushButton("Log In");             // Кнопка для входа.
        auto* registerBtn = new QPushButton("Register");        // Кнопка для регистрации.
        auto* forgotBtn = new QPushButton("Forgot Password");   // Кнопка для восстановления пароля.
        auto* googleBtn = new QPushButton("Sign in with Google"); // Кнопка для входа через Google.

        // Применяем стили к кнопкам.
        loginBtn->setStyleSheet(btnStyle);
        registerBtn->setStyleSheet(btnStyle);
        forgotBtn->setStyleSheet(btnStyle);
        googleBtn->setStyleSheet(googleBtnStyle);

        // Устанавливаем минимальную высоту для кнопок для единообразия.
        int btnHeight = 35;
        loginBtn->setMinimumHeight(btnHeight);
        registerBtn->setMinimumHeight(btnHeight);
        forgotBtn->setMinimumHeight(btnHeight);
        googleBtn->setMinimumHeight(btnHeight);

        // Создаем разделитель "или"
        auto* divider = new QLabel("──────────  or  ──────────");
        divider->setAlignment(Qt::AlignCenter);
        divider->setStyleSheet("color: #777; font-size: 12px;");

        // Создание основного вертикального макета и добавление виджетов
        auto* layout = new QVBoxLayout;
        layout->addWidget(title);
        layout->addWidget(subtitle);
        layout->addSpacing(10);        // Добавляем вертикальный отступ.
        layout->addWidget(email_input);
        layout->addWidget(password_input);
        layout->addSpacing(10);
        layout->addWidget(loginBtn);
        layout->addWidget(registerBtn);
        layout->addWidget(forgotBtn);
        layout->addSpacing(10);
        layout->addWidget(divider);
        layout->addSpacing(10);
        layout->addWidget(googleBtn);
        layout->addStretch();          // Добавляет растягивающееся пространство, чтобы элементы прижимались к верху.
        layout->setContentsMargins(30, 20, 30, 20); // Отступы для всего макета.
        setLayout(layout);             // Устанавливаем этот макет для окна.

        // Установка соединений сигналов и слотов

        // Соединение для кнопки "Log In"
        connect(loginBtn, &QPushButton::clicked, this, [this]() {
            QString email = email_input->text().trimmed(); // Получаем email, удаляя пробелы по краям.
            QString password = password_input->text();     // Получаем пароль.

            // Проверяем, что поля email и пароля не пустые.
            if (email.isEmpty() || password.isEmpty()) {
                QMessageBox::warning(this, "Input Error", "Please enter both email and password.");
                return; // Прерываем выполнение, если поля пустые.
            }

            // Создаем новый экземпляр LogINWindow (предполагается, что это класс для логики входа).
            auto* logInWin = new LogINWindow();
            // Устанавливаем атрибут, чтобы виджет автоматически удалялся при закрытии.
            logInWin->setAttribute(Qt::WA_DeleteOnClose);

            // Соединение, которое будет активировано при уничтожении logInWin.
            // Это позволяет нам проверить, был ли вход успешен, после закрытия окна LogINWindow.
            connect(logInWin, &QObject::destroyed, this, [this, logInWin]() {
                // Проверяем результат операции входа.
                if (logInWin->wasSuccessful()) {
                    emit authSuccess(); // Если успешно, испускаем сигнал об успешной аутентификации.
                    this->close();      // Закрываем окно аутентификации.
                }
            });
            logInWin->onLogInImpl(email, password); // Вызываем метод для попытки входа.
        });

        // Соединение для кнопки "Register"
        connect(registerBtn, &QPushButton::clicked, this, [this]() {
            auto* regWin = new RegisterWindow(); // Создаем новое окно регистрации.
            regWin->setAttribute(Qt::WA_DeleteOnClose); // Автоматическое удаление при закрытии.

            // Соединяем сигнал registrationSuccess из RegisterWindow с лямбда-функцией.
            // Это позволяет нам автоматически войти пользователя после успешной регистрации.
            connect(regWin, &RegisterWindow::registrationSuccess, this,
                    [this](const QString& email, const QString& password) {
                        auto* logInWin = new LogINWindow(); // Создаем окно для логина.
                        logInWin->setAttribute(Qt::WA_DeleteOnClose);

                        // Снова соединяемся с сигналом destroyed, чтобы знать результат входа.
                        connect(logInWin, &QObject::destroyed, this, [this, logInWin]() {
                            if (logInWin->wasSuccessful()) {
                                emit authSuccess();
                                this->close();
                            }
                        });
                        logInWin->onLogInImpl(email, password); // Автоматически пытаемся войти с новыми данными.
                    });

            regWin->show(); // Показываем окно регистрации.
        });

        // Соединение для кнопки "Forgot Password"
        connect(forgotBtn, &QPushButton::clicked, this, [=]() {
            auto* forgotWin = new ForgotPasswordWindow(); // Создаем окно восстановления пароля.
            forgotWin->setAttribute(Qt::WA_DeleteOnClose); // Автоматическое удаление при закрытии.
            forgotWin->show(); // Показываем окно восстановления пароля.
        });

        // Соединение для кнопки "Sign in with Google"
        connect(googleBtn, &QPushButton::clicked, this, &AuthWindow::handleGoogleSignIn);

        // Инициализация TCP-сервера и сетевого менеджера
        server = new QTcpServer(this);       // Создаем локальный TCP-сервер.
        network = new QNetworkAccessManager(this); // Создаем сетевой менеджер.
    }

    // Обработка входа через Google OAuth 2.0
    void AuthWindow::handleGoogleSignIn() {
        startLocalServer(); // Запускаем локальный сервер для перехвата редиректа от Google.

        const QString clientId = "61085252023-kgrp7cksj5vqs6bet9q3ldfaot81nraf.apps.googleusercontent.com";
        const QString redirectUri = "http://localhost:8080";

        // Используем prompt=consent для обязательного показа окна разрешения и получения refresh_token
        QString authUrl = QString("https://accounts.google.com/o/oauth2/v2/auth?"
                                  "client_id=%1&"
                                  "redirect_uri=%2&"
                                  "response_type=code&"
                                  "scope=openid%20email%20profile&"
                                  "access_type=offline&"
                                  "prompt=consent")
                              .arg(clientId, redirectUri);

        QDesktopServices::openUrl(QUrl(authUrl));
    }


    // Запускает локальный TCP-сервер для прослушивания порта 8080.
    // Этот сервер перехватывает HTTP-запрос от браузера после успешной авторизации Google.
    void AuthWindow::startLocalServer() {
        // Соединяем сигнал newConnection (новое входящее соединение) сервера
        // с лямбда-функцией, которая обрабатывает это соединение.
        connect(server, &QTcpServer::newConnection, this, [this]() {
            QTcpSocket* socket = server->nextPendingConnection(); // Получаем сокет для нового соединения.
            if (!socket) return; // Если сокет недействителен, выходим.

            // Ждем, пока данные будут доступны для чтения (с таймаутом 2 секунды).
            if (!socket->waitForReadyRead(2000)) {
                socket->close(); // Закрываем сокет, если данные не получены вовремя.
                socket->deleteLater();
                return;
            }

            QString request = socket->readAll(); // Читаем весь входящий HTTP-запрос.
            // Используем регулярное выражение для поиска кода авторизации в URL запроса.
            // Ожидаем GET-запрос вида "/?code=YOUR_CODE_HERE".
            QRegularExpression re("GET /\\?code=([^&\\s]+)");
            QRegularExpressionMatch match = re.match(request); // Выполняем сопоставление.

            if (match.hasMatch()) { // Если код авторизации найден
                QString code = match.captured(1); // Извлекаем код авторизации.
                exchangeCodeForToken(code);      // Вызываем функцию для обмена кода на токены.

                // Отправляем простой HTTP-ответ в браузер, чтобы сообщить пользователю,
                // что он может закрыть окно.
                socket->write("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                              "<h2>You may close this window now.</h2>");
            } else {
                // Если запрос некорректен или код не найден, отправляем ошибку.
                socket->write("HTTP/1.1 400 Bad Request\r\n\r\nInvalid redirect.");
            }
            socket->disconnectFromHost(); // Отключаем сокет.
            socket->deleteLater();        // Помечаем сокет для последующего удаления.
            server->close();              // Закрываем локальный сервер после получения кода.
        });

        // Пытаемся запустить сервер на прослушивание localhost:8080.
        if (!server->listen(QHostAddress::LocalHost, 8080)) {
            // Если не удалось запустить сервер, показываем сообщение об ошибке.
            QMessageBox::warning(this, "Server Error", "Cannot listen on localhost:8080. "
                                                        "Another application might be using this port.");
        }
    }

    // Обменивает код авторизации, полученный от Google, на access_token и refresh_token.
    void AuthWindow::exchangeCodeForToken(const QString& code) {
        const QString clientId = "61085252023-kgrp7cksj5vqs6bet9q3ldfaot81nraf.apps.googleusercontent.com";
        const QString clientSecret = "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";
        const QString redirectUri = "http://localhost:8080";

        QUrl tokenUrl("https://oauth2.googleapis.com/token");
        QNetworkRequest request(tokenUrl);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        QUrlQuery params;
        params.addQueryItem("code", code);
        params.addQueryItem("client_id", clientId);
        params.addQueryItem("client_secret", clientSecret);
        params.addQueryItem("redirect_uri", redirectUri);
        params.addQueryItem("grant_type", "authorization_code");

        // Первый запрос — обмен кода на токен
        QNetworkReply* exchangeReply = network->post(request, params.toString(QUrl::FullyEncoded).toUtf8());

        connect(exchangeReply, &QNetworkReply::finished, this, [=,this]() {
            if (exchangeReply->error() == QNetworkReply::NoError) {
                QByteArray replyData = exchangeReply->readAll();
                QJsonDocument doc = QJsonDocument::fromJson(replyData);

                QString idToken = doc["id_token"].toString();
                QString refreshToken = doc["refresh_token"].toString();

                if (idToken.isEmpty()) {
                    QMessageBox::warning(this, "Google Login Error", "Missing id_token in response.");
                    exchangeReply->deleteLater();
                    return;
                }

                // Второй запрос — проверка idToken в Firebase
                QUrl firebaseUrl("https://identitytoolkit.googleapis.com/v1/accounts:signInWithIdp?key=AIzaSyAbg8ncf2sH2dRIkfcF6UlcUNdbujSmaQ4");
                QNetworkRequest firebaseRequest(firebaseUrl);
                firebaseRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                QJsonObject payload;
                payload["postBody"] = QString("id_token=%1&providerId=google.com").arg(idToken);
                payload["requestUri"] = "http://localhost";
                payload["returnIdpCredential"] = true;
                payload["returnSecureToken"] = true;

                QNetworkReply* firebaseReply = network->post(firebaseRequest, QJsonDocument(payload).toJson());

                connect(firebaseReply, &QNetworkReply::finished, this, [=,this]() {
                    if (firebaseReply->error() == QNetworkReply::NoError) {
                        // Успешная проверка пользователя
                        QSettings settings("MyCompany", "WeatherApp");
                        settings.setValue("refresh_token", refreshToken);
                        QMessageBox::information(this, "Google Login", "Login successful!");
                        emit authSuccess();
                        this->close();
                    } else {
                        QByteArray err = firebaseReply->readAll();
                        QMessageBox::warning(this, "Firebase Error",
                                             "Google account is not registered in Firebase.\n\n" + err);
                    }
                    firebaseReply->deleteLater();
                });

            } else {
                QByteArray err = exchangeReply->readAll();
                QMessageBox::warning(this, "Google Login Error", exchangeReply->errorString() + "\n\n" + err);
            }
            exchangeReply->deleteLater();
        });
    }
