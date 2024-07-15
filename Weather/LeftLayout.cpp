#include "LeftLayout.h"


LeftLayout::LeftLayout(QWidget* parent)
    : QWidget(parent)
{
    createButtons();
}

void LeftLayout::createButtons()
{
    main_layout = new QVBoxLayout;
    main_layout->addWidget(bt1);
    main_layout->addWidget(bt2);
    main_layout->addWidget(bt3);

}
