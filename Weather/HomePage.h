#pragma once

#include "LeftLayout.h"
#include "RightLayout.h"
#include "CenterLayout.h"
#include "HeaderLayout.h"

#include <QtWidgets>
#include <QLabel>
#include <QSize>
#include <QResizeEvent>

class HomePage : public QWidget
{
    Q_OBJECT
public:
    HomePage(QWidget* parent = nullptr);
public:
    void createLayout();
    void resizeEvent(QResizeEvent* event) override;
    void resizeWidgets();
    double percent(double x, double y);
private:
    QWidget* m_central_widget;
    QGridLayout* main_layout {nullptr};
    HeaderLayout* m_header_layout {nullptr};
    LeftLayout* m_left_layout;
    RightLayout* m_right_layout;
    CenterLayout* m_center_layout;
};
