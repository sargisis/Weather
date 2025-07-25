#pragma once

// INavInterfaceCRTP - это интерфейс для навигационных лейаутов,
// использующий CRTP (Curiously Recurring Template Pattern).
// Это позволяет избавиться от виртуальных функций и добиться максимальной производительности за счёт компиляции.

template<typename Derived>
class INavInterfaceCRTP {
public:
    // Метод, вызываемый извне. Он делегирует вызов функции-реализации в дочернем классе.
    // Позволяет Derived-классу реализовать собственную версию NavBarSelectedOptionButtonsImpl.
    void NavBarSelectedOptionButtons() {
        static_cast<Derived*>(this)->NavBarSelectedOptionButtonsImpl();
    }

    // Виртуальный деструктор — на всякий случай, если класс будет использоваться полиморфно.
    // Хотя при CRTP это редко нужно, наличие не помешает.
    virtual ~INavInterfaceCRTP() = default;
};
