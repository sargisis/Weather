#pragma once

#include "LeftLayout.h"
#include "RightLayout.h"
#include "CenterLayout.h"
#include "HeaderLayout.h"

#include <QtWidgets>
#include <memory>

class HomePage : public QWidget
{
    Q_OBJECT
public:
    explicit HomePage(QWidget* parent = nullptr);

    void createLayout();

private:
    std::unique_ptr<QGridLayout> main_layout;
    std::unique_ptr<HeaderLayout> m_header_layout;
    std::unique_ptr<LeftLayout> m_left_layout;
    std::unique_ptr<RightLayout> m_right_layout;
    std::unique_ptr<CenterLayout> m_center_layout;

    QWidget* leftWidget = nullptr;
    QWidget* centerWidget = nullptr;
    QWidget* rightWidget = nullptr;
};
