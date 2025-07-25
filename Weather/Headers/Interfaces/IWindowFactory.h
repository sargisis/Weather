#ifndef IWINDOWFACTORY_H
#define IWINDOWFACTORY_H

#include <QWidget>
#include <QString>

// Интерфейс фабрики окон, использующий CRTP для статической диспетчеризации
template <typename Derived>
class IWindowFactory
{
public:
    // Создаёт окно входа
    QWidget* createLoginWindow(QWidget* parent = nullptr) {
        return static_cast<Derived*>(this)->createLoginWindowImpl(parent);
    }

    // Создаёт окно регистрации
    QWidget* createRegisterWindow(QWidget* parent = nullptr) {
        return static_cast<Derived*>(this)->createRegisterWindowImpl(parent);
    }

    // Создаёт окно восстановления пароля
    QWidget* createForgotPasswordWindow(QWidget* parent = nullptr) {
        return static_cast<Derived*>(this)->createForgotPasswordWindowImpl(parent);
    }

    // Виртуальный деструктор по-прежнему можно оставить, если нужен полиморфизм позже
    virtual ~IWindowFactory() = default;
};

#endif // IWINDOWFACTORY_H
