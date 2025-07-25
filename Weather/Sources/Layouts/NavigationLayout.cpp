#include "NavigationLayout.h"
#include <QLabel>
#include <QPushButton>
#include <QIcon>

// Конструктор NavigationLayout
// Здесь вызывается функция отрисовки элементов навигации
NavigationLayout::NavigationLayout(QWidget* parent)
    : QVBoxLayout()
{
    NavBarSelectedOptionButtons();  // Отрисовываем кнопки и панель
}

// Основная функция, реализующая отрисовку боковой панели и кнопки её открытия
void NavigationLayout::NavBarSelectedOptionButtonsImpl()
{
    // Кнопка для открытия/закрытия панели
    QPushButton* toggleBtn = new QPushButton("Navigation");
    toggleBtn->setIcon(QIcon(":/icons/list.svg"));  // Иконка меню (гамбургер)
    toggleBtn->setStyleSheet(R"(
        QPushButton {
            font-size: 13px;
            background-color: blue;
            color: white;
            font-weight: bold;
            border-radius: 8px;
            padding: 8px 16px;
        }
        QPushButton:hover {
            background-color: black;
        }
    )");

    // При нажатии на кнопку вызывается слот для анимации панели
    connect(toggleBtn, &QPushButton::clicked, this, &NavigationLayout::toggleExtraPanel);
    this->addWidget(toggleBtn);  // Добавляем кнопку в основной лейаут

    // Создаём боковую панель с кнопками
    m_extraPanel = new QFrame;
    m_extraPanel->setStyleSheet(R"(
        QFrame {
            background-color: #333;
            border: 1px solid #555;
            border-radius: 10px;
        }
    )");
    m_extraPanel->setVisible(false);         // Скрыта по умолчанию
    m_extraPanel->setMaximumWidth(220);      // Ограничиваем максимальную ширину

    // Лейаут для кнопок внутри панели
    auto* panelLayout = new QVBoxLayout(m_extraPanel);
    panelLayout->setContentsMargins(10, 10, 10, 10);
    panelLayout->setSpacing(12);

    // Кнопка: Weather Tools
    QPushButton* weatherBtn = new QPushButton("  Weather Tools");
    weatherBtn->setIcon(QIcon(":/icons/nut.svg"));
    weatherBtn->setStyleSheet(buttonStyle());
    panelLayout->addWidget(weatherBtn);

    // Кнопка: Language Settings
    QPushButton* languageBtn = new QPushButton("  Language Settings");
    languageBtn->setIcon(QIcon(":/icons/globe-simple.svg"));
    languageBtn->setStyleSheet(buttonStyle());
    panelLayout->addWidget(languageBtn);

    // Кнопка: Notifications
    QPushButton* notifyBtn = new QPushButton("  Notifications");
    notifyBtn->setIcon(QIcon(":/icons/bell-simple-ringing.svg"));
    notifyBtn->setStyleSheet(buttonStyle());
    panelLayout->addWidget(notifyBtn);

    // Кнопка: Profile
    QPushButton* profileBtn = new QPushButton("  Profile");
    profileBtn->setIcon(QIcon(":/icons/user.svg"));
    profileBtn->setStyleSheet(buttonStyle());
    panelLayout->addWidget(profileBtn);

    // Добавляем панель и отступ вниз
    this->addWidget(m_extraPanel);
    this->addStretch();  // Прижимает всё вверх
}

// Анимация раскрытия/скрытия боковой панели
void NavigationLayout::toggleExtraPanel()
{
    if (!m_extraPanel) return;

    // Анимация ширины панели
    QPropertyAnimation* animation = new QPropertyAnimation(m_extraPanel, "maximumWidth");
    animation->setDuration(200);
    animation->setStartValue(m_isPanelVisible ? 220 : 0);
    animation->setEndValue(m_isPanelVisible ? 0 : 220);
    animation->setEasingCurve(QEasingCurve::InOutCubic);

    m_isPanelVisible = !m_isPanelVisible;
    m_extraPanel->setVisible(true);  // Всегда делаем видимой, пока идёт анимация

    // После завершения анимации можно скрыть
    connect(animation, &QPropertyAnimation::finished, [this]() {
        if (!m_isPanelVisible) {
            m_extraPanel->setVisible(false);
        }
    });

    animation->start(QAbstractAnimation::DeleteWhenStopped);  // Удалить после завершения
}

// Возвращает стиль для кнопок в боковой панели
QString NavigationLayout::buttonStyle() const
{
    return R"(
        QPushButton {
            background-color: #444;
            color: white;
            font-weight: bold;
            border-radius: 6px;
            padding: 8px 12px;
            text-align: left;
        }
        QPushButton:hover {
            background-color: #555;
        }
    )";
}
