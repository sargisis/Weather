#include "RightLayout.h"

RightLayout::RightLayout(QWidget* parent)
    : QVBoxLayout(parent)

{}

void RightLayout::createRightLayout()
{
    m_right_layout = new QVBoxLayout;
    m_right_label = new QLabel("right label");
    m_right_label->setStyleSheet("background-color: yellow; color: black;");
    addWidget(m_right_label);
}


