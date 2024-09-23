#include "HomePage.h"


HomePage::HomePage(QWidget* parent)
    : QWidget(parent)
{

    createLayout();

    setWindowTitle("Weather-App");

    resize(750,750);
    setMinimumSize(750,750);

    m_left_layout->createLayouts();

    m_right_layout->createLayouts();

    m_center_layout->createLayouts();

    m_header_layout->createInputDialogInHeaderLayout();
}

void HomePage::createLayout()
{
    m_central_widget = new QWidget(this);

    main_layout = new QGridLayout(m_central_widget);

    m_header_layout = new HeaderLayout();

    m_left_layout = new LeftLayout();

    m_right_layout = new RightLayout();

    m_center_layout = new CenterLayout();




    main_layout->addLayout(m_header_layout, 0, 0, 1, 3);

    main_layout->addLayout(m_left_layout, 1, 0);

    main_layout->addLayout(m_center_layout, 1, 1);

    main_layout->addLayout(m_right_layout, 1, 2);

    main_layout->setColumnStretch(0, 1);

    main_layout->setColumnStretch(1, 2);

    main_layout->setColumnStretch(2, 1);

    setLayout(main_layout);
}

void HomePage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    resizeWidgets();
}

void HomePage::resizeWidgets()
{
    main_layout->setColumnStretch(0, percent(this->width(), 20));
    main_layout->setColumnStretch(1, percent(this->width(), 60));
    main_layout->setColumnStretch(2, percent(this->width(), 20));

    m_left_layout->setContentsMargins(10, 0, 10, 0);
    m_right_layout->setContentsMargins(10, 0, 10, 0);
    m_center_layout->setContentsMargins(5, 0, 5, 0);
}

double HomePage::percent(double x, double y)
{
    return x * y / 100;
}




