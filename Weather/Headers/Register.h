#pragma once

// ===== Qt Includes =====
#include <QWidget>                 // Базовый класс для всех виджетов
#include <QLineEdit>              // Текстовые поля (email, password)
#include <QPushButton>            // Кнопка регистрации
#include <QVBoxLayout>            // Вертикальное размещение виджетов
#include <QNetworkAccessManager>  // Для отправки HTTP-запросов
#include <QNetworkReply>          // Для получения ответов
#include <QNetworkRequest>        // Для формирования запроса
#include <QJsonObject>            // JSON тело запроса
#include <QJsonDocument>          // Преобразование JSON <-> строка
#include <QLabel>                 // Надписи, заголовки
#include <QString>                // Строки Qt
#include <QMessageBox>            // Диалоговые окна (ошибки, успех)

// ===== CRTP Interface =====
#include "../Headers/Interfaces/IRegisterWindow.h"

// ===== STL Includes =====
#include <memory>                 // std::unique_ptr

// Класс регистрации нового пользователя через CRTP
//
// ⚠️ Важно: Если вы используете Q_OBJECT, то класс-наследник QWidget
// всегда должен быть ПЕРВЫМ базовым классом!
// Иначе сигналы и слоты работать не будут.
class RegisterWindow : public QWidget, public IRegisterWindow<RegisterWindow>
{
    Q_OBJECT  // макрос Qt для сигналов/слотов

public:
    // Конструктор принимает родителя и инициализирует поля
    explicit RegisterWindow(QWidget* parent = nullptr);

private:
    // ===== UI Components =====
    std::unique_ptr<QLineEdit> email_input;       // Ввод email
    std::unique_ptr<QLineEdit> password_input;    // Ввод пароля
    std::unique_ptr<QLineEdit> confirm_input;     // Подтверждение пароля
    std::unique_ptr<QPushButton> registerBtn;     // Кнопка "Register"

    // ===== Network =====
    std::unique_ptr<QNetworkAccessManager> manager;  // Отправка сетевых запросов

    // ===== CRTP Override =====
    // Метод, вызываемый через базовый интерфейс (CRTP)
    // Реализует логику регистрации через Firebase
    void onRegisterImpl(const QString& email, const QString& password);

signals:
    // Сигнал, который испускается при успешной регистрации
    // Может быть использован для перехода в логин или авто-входа
    void registrationSuccess(const QString& email, const QString& password);
};
