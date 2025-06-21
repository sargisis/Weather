// LeftLayout.cpp
#include "LeftLayout.h"

LeftLayout::LeftLayout(QWidget* parent)
    : QVBoxLayout(parent)
{
    createLayouts();
}

void LeftLayout::createLayouts()
{
    auto* mainCard = new QFrame;
    mainCard->setObjectName("leftCard");
    auto* mainLayout = new QVBoxLayout(mainCard);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    mainCard->setStyleSheet(R"(
        #leftCard {
            background-color: #2c2c2e;
            border-radius: 15px;
        }
        QLabel {
            color: white;
            font-weight: bold;
            font-size: 16px;
        }
        QComboBox, QLineEdit {
            background-color: #3a3a3c;
            color: white;
            padding: 8px;
            border-radius: 8px;
            font-size: 14px;
        }
        QPushButton {
            background-color: #0a84ff;
            color: white;
            font-weight: bold;
            border-radius: 8px;
            padding: 8px 14px;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #0060df;
        }
    )");

    auto* title = new QLabel("🌍 Choose a Country");
    mainLayout->addWidget(title);

    countryDropdown = std::make_unique<QComboBox>();
    mainLayout->addWidget(countryDropdown.get());

    cityInput = std::make_unique<QLineEdit>();
    cityInput->setPlaceholderText("Type a city name...");
    mainLayout->addWidget(cityInput.get());

    searchButton = std::make_unique<QPushButton>("🔍 Search");
    mainLayout->addWidget(searchButton.get());

    connect(searchButton.get(), &QPushButton::clicked, this, &LeftLayout::handleSearch);

    countries = {
        "Afghanistan", "Armenia", "Austria", "Brazil", "Canada", "China",
        "France", "Germany", "India", "Italy", "Japan", "Mexico", "Russia",
        "Spain", "Turkey", "Ukraine", "United Kingdom", "United States"
    };
    countryDropdown->addItems(countries);

    // Scrollable area for city cards
    auto* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    auto* scrollContent = new QWidget;
    cardsLayout = new QVBoxLayout(scrollContent);
    scrollContent->setLayout(cardsLayout);
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);

    this->addWidget(mainCard);
}

void LeftLayout::handleSearch()
{
    QString selectedCountry = countryDropdown->currentText();
    QString city = cityInput->text().trimmed();
    if (!city.isEmpty()) {
        emit countrySelected(selectedCountry, city);
        addCityCard(city);
    }
}

void LeftLayout::addCityCard(const QString& city)
{
    auto* cityCard = new QLabel(QString("📍 %1").arg(city));
    cityCard->setStyleSheet(R"(
        background-color: #444;
        color: white;
        padding: 10px;
        border-radius: 10px;
        font-size: 14px;
    )");
    cityCard->setAlignment(Qt::AlignCenter);
    cardsLayout->addWidget(cityCard);
}
