#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>           // Базовый класс для всех виджетов пользовательского интерфейса
#include <QNetworkAccessManager> // Для выполнения сетевых запросов (к Firebase API)
#include <QString>           // Для работы со строками
#include <memory>            // Для использования std::unique_ptr
#include <QJsonArray>        // Для работы с JSON-массивами (хотя напрямую не используется в LogInUser)
#include <QUrl>              // Для работы с URL-адресами
#include <QJsonObject>       // Для работы с JSON-объектами
#include <QNetworkRequest>   // Для формирования сетевых запросов
#include <QNetworkReply>     // Для обработки ответов от сетевых запросов
#include <QMessageBox>       // Для отображения всплывающих сообщений (успех/ошибка)
#include <QLineEdit>         // Для поля ввода (хотя напрямую не используется в LogINWindow UI)
#include <QVBoxLayout>       // Для вертикального расположения виджетов
#include <QPushButton>       // Для кнопки (хотя напрямую не используется в LogINWindow UI)
#include <QLabel>            // Для отображения текста ("Please wait...")
#include <QGuiApplication>   // Для доступа к QApplication (для получения первичного экрана)
#include <QScreen>           // Для получения информации о экране (для центрирования QMessageBox)


#include "../Headers/Interfaces/ILoginWindow.h" // Включаем заголовочный файл для вашего интерфейса ILoginWindow.

// Класс LogINWindow представляет собой небольшое окно ожидания,
// которое отображается во время выполнения операции входа пользователя.
// Он также инкапсулирует логику отправки запроса на вход к Firebase.
//
// Важное замечание: При множественном наследовании в Qt, класс, который наследуется от QObject (или его потомка, как QWidget),
// должен быть первым в списке наследования, чтобы макрос Q_OBJECT работал корректно.
// Поэтому, `public QWidget, public ILoginWindow` является более предпочтительным.
class LogINWindow : public QWidget, public ILoginWindow<LogINWindow>
{
    Q_OBJECT // Макрос, необходимый для использования сигналов и слотов в Qt (если бы они были здесь)

public:
    // Конструктор класса. Принимает родительский виджет.
    explicit LogINWindow(QWidget* parent = nullptr);

private:
    // Уникальный указатель на QNetworkAccessManager для управления сетевыми запросами.
    std::unique_ptr<QNetworkAccessManager> manager;
    // Указатель на текущий QNetworkReply. Используется для получения ответа от сервера.
    QNetworkReply* reply = nullptr; // Инициализируем nullptr, чтобы избежать неопределенного поведения.
    // Флаг, указывающий, был ли вход пользователя успешным.
    bool loginSuccessful = false;

public:
    // Переопределенный метод из интерфейса ILoginWindow.
    // Отвечает за отправку запроса на вход пользователя к Firebase.
    void onLogInImpl(const QString& email , const QString& password);
    // Метод для проверки, был ли вход успешным.
    bool wasSuccessful() const { return loginSuccessful; }
};

#endif // LOGIN_H
