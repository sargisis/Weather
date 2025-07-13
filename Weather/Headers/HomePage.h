#pragma once

#include "RightLayout.h" // Включаем заголовок для RightLayout (прогноз погоды)
#include "CenterLayout.h" // Включаем заголовок для CenterLayout (текущая погода)
#include "HeaderLayout.h" // Включаем заголовок для HeaderLayout (строка поиска)
#include "Login.h"

#include <QWidget>      // Базовый класс для всех виджетов пользовательского интерфейса
#include <memory>       // Для использования std::unique_ptr для управления памятью макетов
#include <QSettings>

// Класс HomePage представляет собой главное окно вашего погодного приложения.
// Он отвечает за общую компоновку и взаимодействие между различными секциями.
class HomePage : public QWidget
{
    Q_OBJECT // Макрос, необходимый для использования сигналов и слотов в Qt

public:
    // Конструктор класса. Принимает родительский виджет.
    explicit HomePage(QWidget* parent = nullptr);
    // Метод для создания и настройки всех макетов и виджетов на главной странице.
    void createLayout();

private:
    // Уникальные указатели для управления жизненным циклом макетов.
    // std::unique_ptr гарантирует, что объекты будут автоматически удалены,
    // когда HomePage будет уничтожен, предотвращая утечки памяти.
    std::unique_ptr<QGridLayout> main_layout;       // Главный макет окна (сетка).
    std::unique_ptr<HeaderLayout> m_header_layout;  // Макет для верхней части (поиск города).
    std::unique_ptr<RightLayout> m_right_layout;    // Макет для правой части (прогноз на несколько дней).
    std::unique_ptr<CenterLayout> m_center_layout;  // Макет для центральной части (текущая погода).

    // QWidget* для обертывания CenterLayout и RightLayout.
    // Это необходимо, потому что QGridLayout может добавлять только QWidget*,
    // а не QLayout* напрямую (хотя addLayout() есть, но иногда для стилизации
    // или более сложной структуры удобно обернуть QLayout в QWidget).
    QWidget* centerWidget = nullptr; // Виджет-обертка для CenterLayout.
    QWidget* rightWidget = nullptr;  // Виджет-обертка для RightLayout.


signals:
     void requestLogout();
};
