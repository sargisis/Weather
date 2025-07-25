#pragma once

#include <QWidget>           // Базовый класс для всех виджетов пользовательского интерфейса
#include <QVBoxLayout>       // Для вертикального расположения виджетов
#include <QLineEdit>         // Для полей ввода текста (email, пароль)
#include <QPushButton>       // Для кнопок (вход, регистрация, Google Sign-In)
#include <QMessageBox>       // Для отображения всплывающих сообщений (предупреждения, информация)
#include <QNetworkAccessManager> // Для выполнения сетевых запросов (например, для обмена кодом на токен Google)
#include <QNetworkReply>     // Для обработки ответов от сетевых запросов
#include <QJsonObject>       // Для работы с JSON-объектами (парсинг ответов от Google API)
#include <QJsonDocument>     // Для работы с JSON-документами
#include <QLabel>            // Для отображения текста (заголовок, подзаголовок, разделитель)
#include <QDesktopServices>  // Для открытия URL в браузере по умолчанию (для Google Sign-In)
#include <QSettings>         // Для сохранения и загрузки настроек приложения (например, refresh_token)
#include <QUrlQuery>         // Для создания и парсинга URL-параметров запроса
#include <QRegularExpression> // Для работы с регулярными выражениями (парсинг кода авторизации из URL)
#include <QTcpServer>        // Для создания локального TCP-сервера (для получения редиректа от Google)
#include <QTcpSocket>        // Для работы с TCP-сокетами (для обработки входящих соединений на локальном сервере)
#include <QAction>           // Общий класс для действий (хотя здесь напрямую не используется, но часто нужен для виджетов)

// Включаем заголовки для ваших других окон аутентификации.
// Предполагается, что эти классы существуют и корректно определены.
#include "../Headers/Register.h"        // Окно регистрации
#include "../Headers/ForgotPassowrd.h"  // Окно восстановления пароля
#include "../Headers/Login.h"// Окно для логики входа (вероятно, отправляет запросы к вашему API)
#include "GoogleAuthServer.h"

class AuthWindow : public QWidget {
    Q_OBJECT // Макрос, необходимый для использования сигналов и слотов в Qt

public:
    // Конструктор класса. Принимает родительский виджет.
    explicit AuthWindow(QWidget* parent = nullptr);


signals:
    // Сигнал, который испускается, когда аутентификация пользователя прошла успешно.
    // Это позволяет главному окну знать, когда можно показать основной интерфейс приложения.
    void authSuccess();

private:
    // Обрабатывает процесс входа через Google.
    void handleGoogleSignIn();
    // Запускает локальный TCP-сервер для перехвата редиректа от Google OAuth.

    // Обменивает полученный код авторизации Google на токены доступа и обновления.
    void exchangeCodeForToken(const QString& code);
    void startLocalServer();

    QLineEdit* email_input;    // Поле ввода для email.
    QLineEdit* password_input; // Поле ввода для пароля.
    bool passwordVisible = false; // Флаг для отслеживания видимости пароля (если бы была кнопка "показать пароль").

    QTcpServer* server = nullptr; // Указатель на локальный TCP-сервер для Google OAuth.
    QNetworkAccessManager* network = nullptr; // Указатель на менеджер сетевых запросов для Google OAuth.
};
