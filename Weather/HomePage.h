#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include "LeftLayout.h"
#include "RightLayout.h"
#include "CenterLayout.h"

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
private slots:
    void resizeEvent(QResizeEvent *event) override;

    void resizeWidgets(QLayout* layout , const QSize& newSize);
private:

    QWidget* m_central_widget;

    QGridLayout* main_layout {nullptr};

    QHBoxLayout* m_header_layout {nullptr};

    QLabel* m_header_label {nullptr};

    LeftLayout* m_left_layout;

    RightLayout* m_right_layout;

    CenterLayout* m_center_layout;

};

#endif // HOMEPAGE_H
