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

    for (size_t i{ }; i < 20; ++i)
    {
        button = new QPushButton(QString("Button %1").arg(i + 1), containerWidget);
        containerLayout->addWidget(button);
        QObject::connect(button , &QPushButton::clicked , this, &LeftLayout::addButtons);
    }
    addWidget(containerWidget);
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
     else
        {
        QList<QPushButton*> newButtons;
        for (std::size_t i = { 1 }; i <= 3; ++i)
        {
            QPushButton* newButton = new QPushButton(QString("New Button %1").arg(i));
            int index = containerLayout->indexOf(senderButton) + 1;
            containerLayout->insertWidget(index + (i - 1), newButton);
            newButtons.append(newButton);
        }
            addedButtons.insert(senderButton, newButtons);
            clickedbuttons.insert(senderButton);
     }
  }
}
