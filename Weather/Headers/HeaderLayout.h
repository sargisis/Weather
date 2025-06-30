#pragma once

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <memory>

class HeaderLayout : public QHBoxLayout
{
public:
    explicit HeaderLayout(QWidget* parent = nullptr);
    std::unique_ptr<QPushButton> search_button;
    std::unique_ptr<QLineEdit> m_search;

public slots:
    void updateSearchPlaceholder(const QString& country, bool isCityAllowed);
};
