#include "CenterLayout.h"

CenterLayout::CenterLayout(QWidget* parent)
    : QVBoxLayout(parent)
{}


void CenterLayout::createLayouts()
{
    m_center_layout = new QVBoxLayout;
    m_center_label = new QLabel("Center label");
    m_center_label->setStyleSheet("background-color: yellow; color: blue");
    addWidget(m_center_label);

}


