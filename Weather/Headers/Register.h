#pragma once

#include <QWidget>           // Базовый класс для всех виджетов пользовательского интерфейса
#include <QLineEdit>         // Для полей ввода текста (email, пароль, подтверждение пароля)
#include <QPushButton>       // Для кнопки регистрации
#include <QVBoxLayout>       // Для вертикального расположения виджетов
#include <QNetworkAccessManager> // Для выполнения сетевых запросов (к Firebase API)
#include <QNetworkReply>     // Для обработки ответов от сетевых запросов
#include <QString>           // Для работы со строками
#include <QLabel>            // Для отображения текста (заголовок)
#include <QJsonObject>       // Для работы с JSON-объектами
#include <QJsonDocument>     // Для работы с JSON-документами
#include <QNetworkRequest>   // Для формирования сетевых запросов
#include <QMessageBox>       // Для отображения всплывающих сообщений (успех/ошибка)

#include "../Headers/Interfaces/IRegisterWindow.h" // Включаем заголовочный файл для вашего интерфейса IRegisterWindow.
#include <memory>            // Для использования std::unique_ptr

// Класс RegisterWindow представляет собой окно для регистрации нового пользователя.
// Он позволяет пользователю ввести email, пароль и подтверждение пароля,
// а затем отправляет эти данные на сервер Firebase для создания учетной записи.
//
// Важное замечание: При множественном наследовании в Qt, класс, который наследуется от QObject (или его потомка, как QWidget),
// должен быть первым в списке наследования, чтобы макрос Q_OBJECT работал корректно.
// Поэтому, `public QWidget, public IRegisterWindow` является более предпочтительным.
class RegisterWindow : public QWidget, public IRegisterWindow
{
    Q_OBJECT // Макрос, необходимый для использования сигналов и слотов в Qt

public:
    // Конструктор класса. Принимает родительский виджет.
    explicit RegisterWindow(QWidget *parent = nullptr);

private:
    // Уникальные указатели для управления жизненным циклом полей ввода и кнопки.
    // std::unique_ptr гарантирует, что эти объекты будут автоматически удалены,
    // когда RegisterWindow будет уничтожен, предотвращая утечки памяти.
    std::unique_ptr<QLineEdit> email_input;    // Поле ввода для email.
    std::unique_ptr<QLineEdit> password_input; // Поле ввода для пароля.
    std::unique_ptr<QLineEdit> confirm_input;  // Поле для подтверждения пароля.
    std::unique_ptr<QPushButton> registerBtn;  // Кнопка "Register".
    std::unique_ptr<QNetworkAccessManager> manager; // Менеджер для сетевых запросов.

private:
    // Переопределенный метод из интерфейса IRegisterWindow.
    // Отвечает за отправку запроса на регистрацию пользователя к Firebase.
    void RegisterUser(const QString& email , const QString& password) override;

signals:
    // Сигнал, который испускается, когда регистрация пользователя прошла успешно.
    // Передает email и пароль нового пользователя, что может быть использовано,
    // например, для автоматического входа после регистрации.
    void registrationSuccess(const QString& email , const QString& passoword);
};
