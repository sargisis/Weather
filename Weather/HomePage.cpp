#include "HomePage.h"


HomePage::HomePage(QWidget* parent)
    : QWidget(parent)
{

    createLayout();

    setWindowTitle("Weather");


    m_left_layout->createLayouts();

    m_right_layout->createLayouts();

    m_center_layout->createLayouts();


}

void HomePage::createLayout()
{
    m_central_widget = new QWidget(this);

    main_layout = new QGridLayout(m_central_widget);


    m_header_layout = new QHBoxLayout();

    m_header_label = new QLabel("Header Layout");

    m_header_label->setStyleSheet("background-color: blue; color: white;");

    m_header_layout->addWidget(m_header_label);

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



void HomePage::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    QSize newSize = event->size();


    m_header_label->setFixedHeight(newSize.height() * 0.2);


    auto spacing = newSize.width() / 100; //
    m_left_layout->setSpacing(spacing);
    m_right_layout->setSpacing(spacing);
    m_center_layout->setSpacing(spacing);


    resizeWidgets(m_left_layout, newSize);
    resizeWidgets(m_right_layout, newSize);
    resizeWidgets(m_center_layout, newSize);

}

void HomePage::resizeWidgets(QLayout *layout, const QSize &newSize)
{
    for (int i = 0; i < layout->count(); ++i) {
        QWidget* widget = layout->itemAt(i)->widget();
        if (widget) {

            widget->setFixedSize(newSize.width() / 3, newSize.height() / 5);
        }
    }
}
