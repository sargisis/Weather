#include "HeaderLayout.h"

HeaderLayout::HeaderLayout(QWidget* parent)
    : QHBoxLayout(parent) // Initialize the horizontal layout with the specified parent
{
    // Create the search button and set its properties
    search_button = new QPushButton(QString("Search"));
    search_button->setStyleSheet("background-color: blue; color: white;"); // Set button color
    addWidget(search_button); // Add the search button to the layout

    // Create the search input field and set its properties
    m_search = new QLineEdit(parent);
    m_search->setPlaceholderText("Search your city"); // Set placeholder text for the input field
    m_search->setStyleSheet("padding: 5px; font-size: 16px;"); // Set padding and font size
    m_search->setStyleSheet("padding: 5px; font-size: 16px; background-color: darkblue; color: white; border: 1px solid;"); // Set additional styles

    addWidget(m_search); // Add the search input field to the layout
}

void HeaderLayout::updateSearchPlaceholder(const QString &country, bool isCityAllowed)
{
    // Update the placeholder text based on whether the city can be inputted for the specified country
    if (isCityAllowed) {
        m_search->setPlaceholderText("Search city in " + country); // Allow city input
        m_search->setEnabled(true); // Enable the search input field
    } else {
        m_search->setPlaceholderText("Cannot input city for " + country); // Restrict city input
        m_search->clear(); // Clear the input field
        m_search->setEnabled(false); // Disable the search input field
    }
}
