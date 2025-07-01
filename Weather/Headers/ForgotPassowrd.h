#pragma once

#include <QWidget>           // Базовый класс для всех виджетов пользовательского интерфейса
#include <QLineEdit>         // Для поля ввода email
#include <QPushButton>       // Для кнопки отправки
#include <QVBoxLayout>       // Для вертикального расположения виджетов
#include <QNetworkAccessManager> // Для выполнения сетевых запросов (к Firebase API)
#include <QNetworkReply>     // Для обработки ответов от сетевых запросов
#include <QMessageBox>       // Для отображения всплывающих сообщений (успех/ошибка)
#include <QJsonDocument>     // Для работы с JSON-документами (отправка/получение данных)
#include <QJsonObject>       // Для работы с JSON-объектами
#include <QNetworkRequest>   // Для формирования сетевых запросов
#include <QUrl>              // Для работы с URL-адресами
#include <QScreen>           // Для получения информации о экране (для центрирования окна)
#include <QGuiApplication>   // Для доступа к QApplication (для получения первичного экрана)
#include <QLabel>            // Для отображения текста (заголовок)
#include <QStyle>            // (Не используется напрямую в предоставленном коде, но часто включается)

// Включаем заголовочный файл для вашего интерфейса IForgotPassword.
// Это подразумевает, что ForgotPasswordWindow реализует методы, определенные в этом интерфейсе.
#include "../Headers/Interfaces/IForgotPassword.h"

class ForgotPasswordWindow : public QWidget , public IForgotPassword // Наследуемся от QWidget и реализуем IForgotPassword
{
    Q_OBJECT // Макрос, необходимый для использования сигналов и слотов в Qt

public:
    // Конструктор класса. Принимает родительский виджет.
    explicit ForgotPasswordWindow(QWidget *parent = nullptr);

private slots:
    // Переопределенный метод из интерфейса IForgotPassword.
    // Этот слот отвечает за отправку запроса на сброс пароля на сервер Firebase.
    void forgotPasswordUser(const QString& email) override;
    // Слот, который вызывается при нажатии на кнопку "Send Reset Link".
    void onSendClicked();

private:
    QLineEdit* email_input;         // Указатель на поле ввода email.
    QPushButton* sendBtn;           // Указатель на кнопку "Send Reset Link".
    QNetworkAccessManager* manager; // Указатель на менеджер сетевых запросов.
};
