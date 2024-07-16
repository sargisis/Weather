#include "HomePage.h"


HomePage::HomePage(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(700,700);
    createLayout();
    setWindowTitle("Weather");
    m_left_layout->createLeftLayout();
    m_right_layout->createRightLayout();

}

void HomePage::createLayout()
{
    main_layout = new QGridLayout(this);

    m_header_layout = new QHBoxLayout();
    m_header_label = new QLabel("header layout");
    m_header_label->setStyleSheet("background-color: blue; color: white;");
    m_header_label->setFixedSize(700,200);
    m_header_layout->addWidget(m_header_label);


    m_left_layout = new LeftLayout();

    m_right_layout = new RightLayout();

    main_layout->addLayout(m_header_layout , 0 , 0 , 1, 2);
    main_layout->addLayout(m_left_layout , 1 , 0);
    main_layout->addLayout(m_right_layout , 1 , 1);


    main_layout->setColumnStretch(0, 1);
    main_layout->setColumnStretch(1, 2);
}
