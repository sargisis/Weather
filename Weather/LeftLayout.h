// LeftLayout.h
#pragma once

#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStringList>
#include <QFrame>
#include <QScrollArea>
#include <memory>
#include "Layouts.h"

class LeftLayout : public QVBoxLayout, public Layouts
{
    Q_OBJECT
public:
    explicit LeftLayout(QWidget* parent = nullptr);
    void createLayouts() override;

signals:
    void countrySelected(const QString& country, const QString& city);

private slots:
    void handleSearch();

private:
    std::unique_ptr<QComboBox> countryDropdown;
    std::unique_ptr<QLineEdit> cityInput;
    std::unique_ptr<QPushButton> searchButton;

    QVBoxLayout* cardsLayout = nullptr;
    QStringList countries;
    void addCityCard(const QString& city);
};

