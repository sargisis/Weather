#include "NavigationLayout.h"
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QVBoxLayout>

// Конструктор NavigationLayout
NavigationLayout::NavigationLayout(QWidget* parent)
    : QVBoxLayout()
{
    NavBarSelectedOptionButtons();  // Отрисовываем кнопки и панель
}

// Основная функция, реализация боковой панели
void NavigationLayout::NavBarSelectedOptionButtonsImpl()
{
    this->setContentsMargins(10, 10, 10, 10);
    this->setSpacing(20);
    this->setAlignment(Qt::AlignTop);

    // --- Логотип / Название приложения ---
    QLabel* logoLabel = new QLabel("  Meteorite"); 
    logoLabel->setStyleSheet(R"(
        QLabel {
            color: #ffffff;
            font-size: 22px;
            font-weight: 900;
            padding: 10px 5px;
            letter-spacing: 1px;
        }
    )");
    this->addWidget(logoLabel);

    // --- Контейнер для кнопок меню ---
    QFrame* menuFrame = new QFrame;
    menuFrame->setStyleSheet("background: transparent; border: none;");
    auto* panelLayout = new QVBoxLayout(menuFrame);
    panelLayout->setContentsMargins(0, 0, 0, 0);
    panelLayout->setSpacing(10);

    // Вспомогательная лямбда для создания стильных кнопок
    auto createMenuButton = [this](const QString& text, const QString& iconPath) {
        QPushButton* btn = new QPushButton("  " + text);
        if (!iconPath.isEmpty()) {
            btn->setIcon(QIcon(iconPath));
            btn->setIconSize(QSize(20, 20));
        }
        btn->setStyleSheet(this->buttonStyle());
        btn->setCursor(Qt::PointingHandCursor);
        return btn;
    };

    // Создаем кнопки, пытаемся использовать иконки, которые уже были или точно существуют
    QPushButton* weatherBtn = createMenuButton("Dashboard", ":/icons/sun.svg"); 
    QPushButton* mapBtn = createMenuButton("Map", ":/icons/globe-simple.svg");
    QPushButton* notifyBtn = createMenuButton("Notifications", ":/icons/bell-simple-ringing.svg");
    QPushButton* profileBtn = createMenuButton("Settings", ":/icons/user.svg");

    panelLayout->addWidget(weatherBtn);
    panelLayout->addWidget(mapBtn);
    panelLayout->addWidget(notifyBtn);
    panelLayout->addWidget(profileBtn);

    this->addWidget(menuFrame);
    
    // Пружина, чтобы прижать все элементы к верху
    this->addStretch();
}

// Возвращает стиль для кнопок в боковой панели
QString NavigationLayout::buttonStyle() const
{
    // Стиль вкладок (боковое меню) без фона изначально, с hover-эффектом (светлеет)
    return R"(
        QPushButton {
            background-color: transparent;
            color: #b0b0b0;
            font-size: 15px;
            font-weight: bold;
            border-radius: 8px;
            padding: 10px 15px;
            text-align: left;
        }
        QPushButton:hover {
            background-color: #2a2a2a;
            color: #ffffff;
        }
        QPushButton:pressed {
            background-color: #3b3b3b;
        }
    )";
}
