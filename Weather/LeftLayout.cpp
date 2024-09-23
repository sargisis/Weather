#include "LeftLayout.h"

LeftLayout::LeftLayout(QWidget* parent)
    : QVBoxLayout(parent)
{

    createButtonsLeftLayout();
}

void LeftLayout::createLayouts()
{
    m_left_layout = new QVBoxLayout();
}

void LeftLayout::createButtonsLeftLayout()
{
    containerWidget = new QWidget;
    containerLayout = new QVBoxLayout(containerWidget);

    containerLayout->setSpacing(5);
    containerLayout->setContentsMargins(5, 5, 5, 5);

    country_name = {
        "Argentina", "Armenia", "Australia", "Austria", "Bahamas", "Barbados", "Belize", "Bolivia", "Brazil", "Canada", "Chile",
        "Colombia", "Costa Rica", "Cuba", "Dominican Republic", "Ecuador", "El Salvador", "France", "Georgia", "Germany",
        "Greece", "Guatemala", "Honduras", "Hungary", "Iceland", "Ireland", "Italy", "Jamaica", "Mexico", "Netherlands",
        "Nicaragua", "Norway", "Panama", "Paraguay", "Peru", "Poland", "Portugal", "Puerto Rico", "Romania", "Russia",
        "Saint Kitts and Nevis", "Saint Lucia", "Saint Vincent and the Grenadines", "Spain", "Suriname", "Sweden",
        "Switzerland", "Turkey", "United Kingdom", "United States", "Uruguay", "Vatican City", "Venezuela"
    };


    for (std::size_t i { }; i < country_name.size(); ++i)
    {
        QPushButton *button = new QPushButton(country_name[i], containerWidget);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        containerLayout->addWidget(button);
        QObject::connect(button, &QPushButton::clicked, this, &LeftLayout::addButtons);
        QObject::connect(button , &QPushButton::clicked , this , &LeftLayout::selectCountry);
    }

    scroll = new QScrollArea;
    scroll->setWidget(containerWidget);
    scroll->setWidgetResizable(true);

    addWidget(scroll);
}

void LeftLayout::addButtons()
{
    QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
    if (senderButton)
    {
        if (clickedbuttons.contains(senderButton))
        {

            QList<QPushButton*> buttonsToRemove = addedButtons.value(senderButton);
            for (QPushButton* btn : buttonsToRemove)
            {
                containerLayout->removeWidget(btn);
                delete btn;
            }
            addedButtons.remove(senderButton);
            clickedbuttons.remove(senderButton);
        }
    }
}

void LeftLayout::selectCountry()
{

}

