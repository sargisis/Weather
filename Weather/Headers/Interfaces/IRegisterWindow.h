#ifndef IREGISTERWINDOW_H
#define IREGISTERWINDOW_H

// IRegisterWindow — это CRTP-интерфейс для регистрации пользователя.
// Он не использует виртуальные функции, а вместо этого вызывает реализацию в дочернем классе через static_cast.
template <typename Derived>
class IRegisterWindow
{
public:
    // Публичный метод, вызываемый из внешнего кода.
    // Делегирует обработку регистрации дочернему классу через CRTP.
    void onRegister(const QString& email , const QString& password) {
        // Приведение this к типу Derived* и вызов метода реализации из подкласса.
        static_cast<Derived*>(this)->onRegisterImpl(email , password);
    }
};

#endif // IREGISTERWINDOW_H
