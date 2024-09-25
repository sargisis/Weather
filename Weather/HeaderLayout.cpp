#include "HeaderLayout.h"

HeaderLayout::HeaderLayout(QWidget* parent)
    : QHBoxLayout(parent)
{

    search_button = new QPushButton(QString("Search"));
    search_button->setStyleSheet("background-color: blue; color: white;");
    addWidget(search_button);

    m_search = new QLineEdit(parent);
    m_search->setPlaceholderText("Search your city");
    m_search->setStyleSheet("padding: 5px; font-size: 16px;");
    addWidget(m_search);
}


void HeaderLayout::updateSearchPlaceholder(const QString &country, bool isCityAllowed)
{
    if (isCityAllowed) {
    m_search->setPlaceholderText("Search city in " + country);
    m_search->setEnabled(true);
    } else {
        m_search->setPlaceholderText("Нельзя вводить города для " + country);
        m_search->clear();
        m_search->setEnabled(false);
    }
}
