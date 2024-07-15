#include "HomePage.h"
#include "LeftLayout.h"

HomePage::HomePage(QWidget* parent)
    : QWidget(parent)
{
    setFixedSize(700,700);
    createLayout();
    setWindowTitle("Weather");
}

void HomePage::createLayout()
{
    main_layout = new QGridLayout(this);
    header_layout = new QHBoxLayout();
    header_label = new QLabel("header");
    header_label->setStyleSheet("background-color: blue; color: white;");
    header_label->setFixedSize(700,200);
    header_layout->addWidget(header_label);


    // left Layout
    left_layout = new QVBoxLayout;
    left_label = new QLabel("left_label");
    left_label->setStyleSheet("background-color: green; color: white;");
    left_layout->addWidget(left_label);

    // right Layout
    right_layout = new QVBoxLayout;
    right_label = new QLabel("   right_label");
    right_label->setStyleSheet("background-color: yellow; color: black;");
    right_layout->addWidget(right_label);

    main_layout->addLayout(header_layout , 0 , 0 , 1, 2);
    main_layout->addLayout(left_layout , 1 , 0);
    main_layout->addLayout(right_layout , 1 , 1);


    main_layout->setColumnStretch(0, 1);
    main_layout->setColumnStretch(1, 2);
}
