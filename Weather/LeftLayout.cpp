#include "LeftLayout.h"

LeftLayout::LeftLayout(QWidget* parent)
    : QVBoxLayout(parent)
{}

void LeftLayout::createLeftLayout()
{
    m_left_layout = new QVBoxLayout;
    m_left_label = new QLabel("left label");
    m_left_label->setStyleSheet("background-color: yellow; color: blue");
    addWidget(m_left_label);
}
