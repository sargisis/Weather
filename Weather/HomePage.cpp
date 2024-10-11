#include "HomePage.h"

HomePage::HomePage(QWidget* parent)
    : QWidget(parent) // Initialize the QWidget with the specified parent
{
    createLayout(); // Create the layout for the main window
    setWindowTitle("Weather-App"); // Set the window title
    resize(750, 750); // Set the initial window size
    setMinimumSize(750, 750); // Set the minimum window size to prevent resizing below a certain point

    QString styleSheet = R"(
        QWidget {
            background-color: #1c1c1c; /* Тёмный фон */
            color: #ffffff; /* Светлый текст */
        }
        QLineEdit {
            background-color: #2a2a2a;
            border: 2px solid #3e3e3e;
            border-radius: 10px;
            padding: 10px;
            color: #ffffff;
            font-size: 16px;
        }
        QPushButton {
            background-color: #007BFF;
            color: white;
            font-weight: bold;
            border-radius: 10px;
            padding: 10px 20px;
            font-size: 16px;
        }
        QPushButton:hover {
            background-color: #0056b3;
        }
        QLabel {
            font-size: 20px;
            font-weight: bold;
            color: #ffffff;
        }
        QListWidget {
            background-color: #2a2a2a;
            border-radius: 10px;
            padding: 10px;
            color: #ffffff;
        }
        /* Тени для панелей */
        .left-panel, .right-panel, .center-panel {
            background-color: #2b2b2b;
            border-radius: 10px;
            padding: 20px;
            box-shadow: 0px 4px 10px rgba(0, 0, 0, 0.5);
        }
        /* Карточки для прогноза погоды */
        .forecast-card {
            background-color: #333333;
            border-radius: 8px;
            padding: 15px;
            margin-bottom: 10px;
        }
        .forecast-card:hover {
            background-color: #444444;
        }
    )";

    setStyleSheet(styleSheet);


    // Create layouts for the left, right, and center sections of the main window
    m_left_layout->createLayouts();
    m_right_layout->createLayouts();
    m_center_layout->createLayouts();

    // Connect the country selection signal from the left layout to update the search placeholder in the header
    connect(m_left_layout, &LeftLayout::countrySelected, this, [this](const QString& country, bool isCityAllowed) {
        m_header_layout->updateSearchPlaceholder(country, isCityAllowed);
        m_header_layout->m_search->setEnabled(isCityAllowed); // Enable or disable the search input based on city availability
    });

    // Connect the search button click signal to fetch weather data for the entered city
    connect(m_header_layout->search_button, &QPushButton::clicked, this, [this]() {
        QString city = m_header_layout->m_search->text(); // Get the city name from the search input
        if (!city.isEmpty()) { // Check if the search bar is not empty
            m_center_layout->fetchWeatherDataForCity(city); // Fetch weather data for the specified city
            m_right_layout->fetchFutureWeather(city); // Fetch future weather data for the specified city
        } else {
            qDebug() << "Search bar is empty"; // Debug message if the search bar is empty
        }
    });
}

void HomePage::createLayout()
{
    m_central_widget = new QWidget(this); // Create a central widget for the main layout
    main_layout = new QGridLayout(m_central_widget); // Create a grid layout for organizing child widgets
    m_header_layout = new HeaderLayout(); // Instantiate the header layout
    m_left_layout = new LeftLayout(); // Instantiate the left layout
    m_right_layout = new RightLayout(); // Instantiate the right layout
    m_center_layout = new CenterLayout(); // Instantiate the center layout

    // Add the header and layout sections to the main grid layout
    main_layout->addLayout(m_header_layout, 0, 0, 1, 3); // Header spans all three columns
    main_layout->addLayout(m_left_layout, 1, 0); // Left layout in column 0
    main_layout->addLayout(m_center_layout, 1, 1); // Center layout in column 1
    main_layout->addLayout(m_right_layout, 1, 2); // Right layout in column 2

    // Set column stretch factors to control the width of each column
    main_layout->setColumnStretch(0, 1);
    main_layout->setColumnStretch(1, 2);
    main_layout->setColumnStretch(2, 1);
    setLayout(main_layout); // Set the main layout for the central widget
}

void HomePage::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event); // Call the base class's resize event
    resizeWidgets(); // Adjust widget sizes based on the new window size
}

void HomePage::resizeWidgets()
{
    // Set column stretch factors based on the new width of the window
    main_layout->setColumnStretch(0, percent(this->width(), 20)); // Left column takes 20% of the width
    main_layout->setColumnStretch(1, percent(this->width(), 60)); // Center column takes 60% of the width
    main_layout->setColumnStretch(2, percent(this->width(), 20)); // Right column takes 20% of the width

    // Set contents margins for the left, right, and center layouts for better spacing
    m_left_layout->setContentsMargins(10, 0, 10, 0);
    m_right_layout->setContentsMargins(10, 0, 10, 0);
    m_center_layout->setContentsMargins(5, 0, 5, 0);
}

double HomePage::percent(double x, double y)
{
    return x * y / 100; // Calculate the percentage of x based on y
}
