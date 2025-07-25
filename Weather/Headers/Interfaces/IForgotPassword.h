#ifndef IFORGOTPASSWORD_H
#define IFORGOTPASSWORD_H

#include <QString>

// CRTP (Curiously Recurring Template Pattern) интерфейс для восстановления пароля
// Класс IForgotPassword принимает Derived — это тип класса-наследника
template <typename Derived>
class IForgotPassword
{
public:
    // Метод, вызываемый извне. Принимает email и делегирует вызов в реализацию в классе-наследнике.
    // Вместо виртуальной функции мы используем статическое приведение типа через CRTP.
    void onForgotPassword(const QString& email) {
        // static_cast приводит текущий объект к типу наследника,
        // чтобы вызвать метод onForgotPasswordImpl, который должен быть реализован в наследнике.
        static_cast<Derived*>(this)->onForgotPasswordImpl(email);
    }

    // Здесь не определена никакая виртуальная функция — это позволяет избежать затрат
    // на vtable и делает вызов функции компиляторно-инлайновым, то есть быстрым.
};

#endif // IFORGOTPASSWORD_H
