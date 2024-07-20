#include "LeftLayout.h"

LeftLayout::LeftLayout(QWidget* parent)
    : QVBoxLayout(parent)
{

    createButtonsLeftLayout();
}

void LeftLayout::createLayouts()
{
    m_left_layout = new QVBoxLayout;
}


void LeftLayout::createButtonsLeftLayout()
{
     containerWidget = new QWidget;
     containerLayout = new QVBoxLayout(containerWidget);

    containerLayout->setSpacing(5);
    containerLayout->setContentsMargins(5, 5, 5, 5);

    for (size_t i{}; i < 20; ++i)
    {
        button = new QPushButton(QString("Button %1").arg(i + 1) , containerWidget);
        // button->setStyleSheet("background-color: red; color: white");
        containerLayout->addWidget(button);
    }
    addWidget(containerWidget);



}
