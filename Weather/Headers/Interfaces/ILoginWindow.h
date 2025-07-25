#ifndef ILOGINWINDOW_H
#define ILOGINWINDOW_H

#include <QString>  // Не забудь включить, если используешь QString

// Интерфейс для окна входа с использованием CRTP
// CRTP позволяет избежать виртуальных функций и сделать вызовы методов статически (во время компиляции),
// что устраняет накладные расходы на виртуальные таблицы (vtable).
template <typename Derived>
class ILoginWindow
{
public:
    // Метод, вызываемый снаружи для входа пользователя.
    // Он вызывает реализацию в классе-наследнике (Derived),
    // используя static_cast для компиляторного разрешения метода onLogInImpl.
    void onLogIn(const QString& email, const QString& password) {
        static_cast<Derived*>(this)->onLogInImpl(email, password);
    }
};

#endif // ILOGINWINDOW_H
