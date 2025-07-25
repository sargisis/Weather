#pragma once

#include "RightLayout.h"
#include "CenterLayout.h"
#include "HeaderLayout.h"
#include "NavigationLayout.h"
#include "Login.h"

#include <QWidget>
#include <memory>
#include <QSettings>
#include <QPushButton>
#include <QFrame>
#include <QHBoxLayout>
#include <QGridLayout>

// Класс HomePage представляет основное окно приложения после авторизации.
// Содержит разметку: заголовок (HeaderLayout), центральный блок, правую панель и навигационное меню.
class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget* parent = nullptr);

    // Создает и инициализирует все вложенные лэйауты и виджеты.
    void createLayout();

private:
    // Главный макет для размещения всех блоков по сетке.
    std::unique_ptr<QGridLayout> main_layout;

    // Верхняя строка с поиском и другими контролами.
    std::unique_ptr<HeaderLayout> m_header_layout;

    // Правая панель с будущей погодой и дополнительной информацией.
    std::unique_ptr<RightLayout> m_right_layout;

    // Центральная панель с текущей погодой.
    std::unique_ptr<CenterLayout> m_center_layout;

    // Навигационное меню слева.
    std::unique_ptr<NavigationLayout> m_navigation_layout;

    // Обертки для лэйаутов — используются как контейнеры для установки на grid layout.
    QWidget* centerWidget = nullptr;
    QWidget* rightWidget = nullptr;
    QWidget* navigationWidget = nullptr;
};
