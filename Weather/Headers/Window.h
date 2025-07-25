#ifndef WINDOW_FACTORY_H
#define WINDOW_FACTORY_H

#include "./Interfaces/IWindowFactory.h" // Подключение CRTP-интерфейса фабрики окон
#include "Register.h"                    // Окно регистрации
#include "Login.h"                       // Окно входа
#include "ForgotPassowrd.h"              // Окно восстановления пароля

// Класс WindowFactory реализует интерфейс IWindowFactory с помощью CRTP.
// Он создаёт конкретные реализации окон (Login, Register, ForgotPassword),
// не используя виртуальные функции (без vtable), что повышает производительность.
class WindowFactory : public IWindowFactory<WindowFactory>
{
public:
    // Метод для создания окна входа
    QWidget* createLoginWindowImpl(QWidget* parent = nullptr) {
        return new LoginWindow(parent);
    }

    // Метод для создания окна регистрации
    QWidget* createRegisterWindowImpl(QWidget* parent = nullptr) {
        return new RegisterWindow(parent);
    }

    // Метод для создания окна восстановления пароля
    QWidget* createForgotPasswordWindowImpl(QWidget* parent = nullptr) {
        return new ForgotPasswordWindow(parent);
    }
};

#endif // WINDOW_FACTORY_H
