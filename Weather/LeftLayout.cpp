#include "LeftLayout.h"

LeftLayout::LeftLayout(QWidget* parent)
    : QVBoxLayout(parent) // Initialize the QVBoxLayout with the specified parent
{
    createButtonsLeftLayout(); // Create buttons for the left layout
}

void LeftLayout::createLayouts()
{
    m_left_layout = new QVBoxLayout(); // Create a new vertical layout for left layout components
}

void LeftLayout::createButtonsLeftLayout()
{
    containerWidget = new QWidget; // Create a container widget to hold the buttons
    containerLayout = new QVBoxLayout(containerWidget); // Create a vertical layout for the container widget

    // Set spacing and margins for the container layout
    containerLayout->setSpacing(5);
    containerLayout->setContentsMargins(10, 10, 10, 10);

    // List of country names to create buttons for
    country_name = {
                    "Afghanistan", "Albania", "Algeria", "Andorra", "Angola",
                    "Antigua and Barbuda", "Argentina", "Armenia", "Australia",
                    "Austria", "Azerbaijan", "Bahamas", "Bahrain", "Bangladesh",
                    "Barbados", "Belarus", "Belgium", "Belize", "Benin",
                    "Bhutan", "Bolivia", "Bosnia and Herzegovina", "Botswana",
                    "Brazil", "Brunei", "Bulgaria", "Burkina Faso", "Burundi",
                    "Cabo Verde", "Cambodia", "Cameroon", "Canada",
                    "Central African Republic", "Chad", "Chile", "China",
                    "Colombia", "Comoros", "Congo (Congo-Brazzaville)",
                    "Costa Rica", "Croatia", "Cuba", "Cyprus", "Czech Republic",
                    "Democratic Republic of the Congo", "Denmark", "Djibouti",
                    "Dominica", "Dominican Republic", "Ecuador", "Egypt",
                    "El Salvador", "Equatorial Guinea", "Eritrea", "Estonia",
                    "Eswatini", "Ethiopia", "Fiji", "Finland", "France",
                    "Gabon", "Gambia", "Georgia", "Germany", "Ghana",
                    "Greece", "Grenada", "Guatemala", "Guinea", "Guinea-Bissau",
                    "Guyana", "Haiti", "Honduras", "Hungary", "Iceland",
                    "India", "Indonesia", "Iran", "Iraq", "Ireland",
                    "Israel", "Italy", "Jamaica", "Japan", "Jordan",
                    "Kazakhstan", "Kenya", "Kiribati", "Kuwait", "Kyrgyzstan",
                    "Laos", "Latvia", "Lebanon", "Lesotho", "Liberia",
                    "Libya", "Liechtenstein", "Lithuania", "Luxembourg",
                    "Madagascar", "Malawi", "Malaysia", "Maldives", "Mali",
                    "Malta", "Marshall Islands", "Mauritania", "Mauritius",
                    "Mexico", "Micronesia", "Moldova", "Monaco", "Mongolia",
                    "Montenegro", "Morocco", "Mozambique", "Myanmar",
                    "Namibia", "Nauru", "Nepal", "Netherlands", "New Zealand",
                    "Nicaragua", "Niger", "Nigeria", "North Korea",
                    "North Macedonia", "Norway", "Oman", "Pakistan",
                    "Palau", "Palestine State", "Panama", "Papua New Guinea",
                    "Paraguay", "Peru", "Philippines", "Poland", "Portugal",
                    "Qatar", "Romania", "Russia", "Rwanda", "Saint Kitts and Nevis",
                    "Saint Lucia", "Saint Vincent and the Grenadines",
                    "Samoa", "San Marino", "Sao Tome and Principe", "Saudi Arabia",
                    "Senegal", "Serbia", "Seychelles", "Sierra Leone",
                    "Singapore", "Slovakia", "Slovenia", "Solomon Islands",
                    "Somalia", "South Africa", "South Korea", "South Sudan",
                    "Spain", "Sri Lanka", "Sudan", "Suriname", "Sweden",
                    "Switzerland", "Syria", "Tajikistan", "Tanzania",
                    "Thailand", "Timor-Leste", "Togo", "Tonga", "Trinidad and Tobago",
                    "Tunisia", "Turkey", "Turkmenistan", "Tuvalu",
                    "Uganda", "Ukraine", "United Arab Emirates",
                    "United Kingdom", "United States", "Uruguay",
                    "Uzbekistan", "Vanuatu", "Vatican City", "Venezuela",
                    "Vietnam", "Yemen", "Zambia", "Zimbabwe"
    };

    // Create buttons for each country and connect their signals
    for (std::size_t i {}; i < country_name.size(); ++i)
    {
        QPushButton* button = new QPushButton(country_name[i], containerWidget); // Create a new button for the country
        button->setMinimumWidth(100);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // Set size policy for the button
        containerLayout->addWidget(button); // Add the button to the container layout

        // Connect button signals to their respective slots
        QObject::connect(button, &QPushButton::clicked, this, &LeftLayout::addButtons);
        QObject::connect(button, &QPushButton::clicked, this, &LeftLayout::selectCountry);
    }

    // Create a scroll area to contain the buttons
    scroll = new QScrollArea;
    scroll->setWidget(containerWidget); // Set the container widget as the scroll area widget
    scroll->setWidgetResizable(true); // Make the widget resizable within the scroll area
    scroll->setFixedHeight(600); // Set a fixed height for the scroll area

    addWidget(scroll); // Add the scroll area to the left layout
}

void LeftLayout::addButtons()
{
    // Get the button that triggered this slot
    QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
    if (senderButton)
    {
        // Check if the button has already been clicked
        if (clickedbuttons.contains(senderButton))
        {
            // Remove and delete buttons associated with the sender button
            QList<QPushButton*> buttonsToRemove = addedButtons.value(senderButton);
            for (QPushButton* btn : buttonsToRemove)
            {
                containerLayout->removeWidget(btn); // Remove the button from the layout
                delete btn; // Delete the button to free memory
            }
            addedButtons.remove(senderButton); // Remove the sender from the added buttons map
            clickedbuttons.remove(senderButton); // Remove the sender from the clicked buttons list
        }
    }
}

void LeftLayout::selectCountry()
{
    // Get the button that triggered this slot
    QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
    if (senderButton)
    {
        QString selectedCountry = senderButton->text(); // Get the text of the clicked button (country name)
        bool canInputCity = canInputCityForCountry(selectedCountry); // Check if a city can be input for the selected country
        emit countrySelected(selectedCountry, canInputCity); // Emit the signal with the selected country and city input allowance
    }
}

bool LeftLayout::canInputCityForCountry(const QString& country)
{
    // List of countries that allow city input
    QStringList allowedCountries = {
                                    "Afghanistan", "Albania", "Algeria", "Andorra", "Angola",
                                    "Antigua and Barbuda", "Argentina", "Armenia", "Australia",
                                    "Austria", "Azerbaijan", "Bahamas", "Bahrain", "Bangladesh",
                                    "Barbados", "Belarus", "Belgium", "Belize", "Benin",
                                    "Bhutan", "Bolivia", "Bosnia and Herzegovina", "Botswana",
                                    "Brazil", "Brunei", "Bulgaria", "Burkina Faso", "Burundi",
                                    "Cabo Verde", "Cambodia", "Cameroon", "Canada",
                                    "Central African Republic", "Chad", "Chile", "China",
                                    "Colombia", "Comoros", "Congo (Congo-Brazzaville)",
                                    "Costa Rica", "Croatia", "Cuba", "Cyprus", "Czech Republic",
                                    "Democratic Republic of the Congo", "Denmark", "Djibouti",
                                    "Dominica", "Dominican Republic", "Ecuador", "Egypt",
                                    "El Salvador", "Equatorial Guinea", "Eritrea", "Estonia",
                                    "Eswatini", "Ethiopia", "Fiji", "Finland", "France",
                                    "Gabon", "Gambia", "Georgia", "Germany", "Ghana",
                                    "Greece", "Grenada", "Guatemala", "Guinea", "Guinea-Bissau",
                                    "Guyana", "Haiti", "Honduras", "Hungary", "Iceland",
                                    "India", "Indonesia", "Iran", "Iraq", "Ireland",
                                    "Israel", "Italy", "Jamaica", "Japan", "Jordan",
                                    "Kazakhstan", "Kenya", "Kiribati", "Kuwait", "Kyrgyzstan",
                                    "Laos", "Latvia", "Lebanon", "Lesotho", "Liberia",
                                    "Libya", "Liechtenstein", "Lithuania", "Luxembourg",
                                    "Madagascar", "Malawi", "Malaysia", "Maldives", "Mali",
                                    "Malta", "Marshall Islands", "Mauritania", "Mauritius",
                                    "Mexico", "Micronesia", "Moldova", "Monaco", "Mongolia",
                                    "Montenegro", "Morocco", "Mozambique", "Myanmar",
                                    "Namibia", "Nauru", "Nepal", "Netherlands", "New Zealand",
                                    "Nicaragua", "Niger", "Nigeria", "North Korea",
                                    "North Macedonia", "Norway", "Oman", "Pakistan",
                                    "Palau", "Palestine State", "Panama", "Papua New Guinea",
                                    "Paraguay", "Peru", "Philippines", "Poland", "Portugal",
                                    "Qatar", "Romania", "Russia", "Rwanda", "Saint Kitts and Nevis",
                                    "Saint Lucia", "Saint Vincent and the Grenadines",
                                    "Samoa", "San Marino", "Sao Tome and Principe", "Saudi Arabia",
                                    "Senegal", "Serbia", "Seychelles", "Sierra Leone",
                                    "Singapore", "Slovakia", "Slovenia", "Solomon Islands",
                                    "Somalia", "South Africa", "South Korea", "South Sudan",
                                    "Spain", "Sri Lanka", "Sudan", "Suriname", "Sweden",
                                    "Switzerland", "Syria", "Tajikistan", "Tanzania",
                                    "Thailand", "Timor-Leste", "Togo", "Tonga", "Trinidad and Tobago",
                                    "Tunisia", "Turkey", "Turkmenistan", "Tuvalu",
                                    "Uganda", "Ukraine", "United Arab Emirates",
                                    "United Kingdom", "United States", "Uruguay",
                                    "Uzbekistan", "Vanuatu", "Vatican City", "Venezuela",
                                    "Vietnam", "Yemen", "Zambia", "Zimbabwe"
    };
    return allowedCountries.contains(country); // Check if the country is in the allowed list
}
