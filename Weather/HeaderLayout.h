#pragma once

#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMap>
#include <QMessageBox>

#include "CenterLayout.h"

class HeaderLayout : public QHBoxLayout
{
public:
    HeaderLayout(QWidget* parent = nullptr);
public:
    QPushButton* search_button {nullptr};
    QLineEdit* m_search;
public slots:
    void updateSearchPlaceholder(const QString& country, bool isCityAllowed);
};
