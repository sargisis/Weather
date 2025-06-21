#include "HeaderLayout.h"

HeaderLayout::HeaderLayout(QWidget* parent)
    : QHBoxLayout(parent),
    search_button(std::make_unique<QPushButton>("Search")),
    m_search(std::make_unique<QLineEdit>())
{
    search_button->setStyleSheet(R"(
        QPushButton {
            background-color: #007BFF;
            color: white;
            font-weight: bold;
            border-radius: 8px;
            padding: 8px 20px;
        }
        QPushButton:hover {
            background-color: #0056b3;
        }
    )");
    m_search->setStyleSheet(R"(
        QLineEdit {
            padding: 8px;
            font-size: 16px;
            border-radius: 6px;
            background-color: #1e3a8a;
            border: 1px solid #333;
            color: white;
        }
    )");

    m_search->setPlaceholderText("Search your city");

    addWidget(search_button.get(), 0);
    addWidget(m_search.get(), 1);
}

void HeaderLayout::updateSearchPlaceholder(const QString& country, bool isCityAllowed)
{
    if (isCityAllowed) {
        m_search->setPlaceholderText("Search city in " + country);
        m_search->setEnabled(true);
    } else {
        m_search->setPlaceholderText("Cannot input city for " + country);
        m_search->clear();
        m_search->setEnabled(false);
    }
}
