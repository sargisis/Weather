#include "HomePage.h"

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

    // ✅ Connect signal AFTER layouts are ready
    connect(m_left_layout.get(), &LeftLayout::countrySelected, this,
            [this](const QString& country, const QString& city) {
                qDebug() << "Country:" << country << ", City:" << city;
                if (!city.isEmpty()) {
                    m_center_layout->fetchWeatherDataForCity(city);
                    m_right_layout->fetchFutureWeather(city);
                }
            });
}


    void HomePage::createLayout()
{
    main_layout = std::make_unique<QGridLayout>();

    m_header_layout = std::make_unique<HeaderLayout>();
    m_left_layout = std::make_unique<LeftLayout>();
    m_center_layout = std::make_unique<CenterLayout>();
    m_right_layout = std::make_unique<RightLayout>();

    // Оборачиваем layouts в QWidget вручную
    leftWidget = new QWidget(this);
    {
        auto* wrapperLayout = new QVBoxLayout(leftWidget);
        wrapperLayout->addLayout(m_left_layout.get());
        wrapperLayout->setContentsMargins(0, 0, 0, 0);
    }

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

    main_layout->addLayout(m_header_layout.get(), 0, 0, 1, 3);
    main_layout->addWidget(leftWidget, 1, 0);
    main_layout->addWidget(centerWidget, 1, 1);
    main_layout->addWidget(rightWidget, 1, 2);

    main_layout->setColumnStretch(0, 1);
    main_layout->setColumnStretch(1, 2);
    main_layout->setColumnStretch(2, 1);
    main_layout->setContentsMargins(20, 20, 20, 20);
    main_layout->setHorizontalSpacing(15);
    main_layout->setVerticalSpacing(10);

    setLayout(main_layout.get());
}


