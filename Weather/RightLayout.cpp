#include "RightLayout.h"

RightLayout::RightLayout(QWidget* parent)
    : QVBoxLayout(parent)

{}

void RightLayout::createLayouts()
{
    m_right_layout = new QVBoxLayout;
    m_right_label = new QLabel("right label"); // Remove Soon
    m_right_label->setStyleSheet("background-color: yellow; color: black;"); // Remove Soon
    addWidget(m_right_label);

}


