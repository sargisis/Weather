#include "../Headers/HomePage.h"
#include <QScreen>
#include <QGuiApplication>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include <QLabel>

HomePage::HomePage(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Weather-App");
    resize(900, 750);
    setMinimumSize(800, 600);

    setStyleSheet(R"(
        QWidget { background-color: #1c1c1c; color: #ffffff; }
        QLineEdit {
            background-color: #2a2a2a; border: 2px solid #3e3e3e;
            border-radius: 10px; padding: 10px; color: #ffffff; font-size: 16px;
        }
        QPushButton {
            background-color: #007BFF; color: white; font-weight: bold;
            border-radius: 10px; padding: 10px 20px; font-size: 16px;
        }
        QPushButton:hover { background-color: #0056b3; }
        QLabel { font-size: 16px; color: #ffffff; }
    )");

    createLayout();

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

void HomePage::createLayout()
{
    main_layout = std::make_unique<QGridLayout>();
    m_header_layout = std::make_unique<HeaderLayout>();
    m_center_layout = std::make_unique<CenterLayout>();
    m_right_layout = std::make_unique<RightLayout>();
    m_navigation_layout = std::make_unique<NavigationLayout>();

    centerWidget = new QWidget(this);
    {
        auto* wrapperLayout = new QVBoxLayout(centerWidget);
        wrapperLayout->addLayout(m_center_layout.get());
        wrapperLayout->setContentsMargins(0, 0, 0, 0);
    }

    rightWidget = new QWidget(this);
    {
        auto* wrapperLayout = new QVBoxLayout(rightWidget);
        wrapperLayout->addLayout(m_right_layout.get());
        wrapperLayout->setContentsMargins(0, 0, 0, 0);
    }

    navigationWidget = new QWidget(this);
    {
        auto* wrapperLayout = new QVBoxLayout(navigationWidget);
        wrapperLayout->addLayout(m_navigation_layout.get());
        wrapperLayout->setContentsMargins(0, 0, 0, 0);
    }

    connect(m_header_layout->m_search.get(), &QLineEdit::returnPressed, this, [this]() {
        const QString city = m_header_layout->m_search->text().trimmed();
        if (!city.isEmpty()) {
            m_center_layout->fetchWeatherDataForCity(city);
            m_right_layout->fetchFutureWeather(city);
        }
    });

    main_layout->addLayout(m_header_layout.get(), 0, 0, 1, 3);
    main_layout->addWidget(navigationWidget, 1, 0);
    main_layout->addWidget(centerWidget, 1, 1);
    main_layout->addWidget(rightWidget, 1, 2);

    main_layout->setColumnStretch(0, 1);
    main_layout->setColumnStretch(1, 3);
    main_layout->setColumnStretch(2, 2);
    main_layout->setContentsMargins(20, 20, 20, 20);
    main_layout->setHorizontalSpacing(15);
    main_layout->setVerticalSpacing(10);

    setLayout(main_layout.get());
}

