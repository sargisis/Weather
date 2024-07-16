#include "HeaderLayout.h"

HeaderLayout::HeaderLayout(QWidget* parent)
    : QHBoxLayout(parent)
{}


void HeaderLayout::createHeaderLayout()
{
    m_header_layout = new QHBoxLayout;
    m_header_label = new QLabel("header layout");
    m_header_label->setFixedSize(700 ,200);
    m_header_label->setStyleSheet("background-color: blue; color: white;");
    addWidget(m_header_label);
}
