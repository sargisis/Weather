#pragma once

#include <cstddef>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QString>
#include <QSet>
#include <QMap>
#include <QList>
#include <QStringList>
#include <QScrollArea>
#include <QStringList>

#include "Layouts.h"
#include "HeaderLayout.h"


class LeftLayout : public QVBoxLayout , public Layouts
{
      Q_OBJECT
public:
   explicit LeftLayout(QWidget* parent = nullptr);

public:
    virtual void createLayouts() override;
    void createButtonsLeftLayout();
    bool canInputCityForCountry(const QString& country);

private slots:
    void addButtons();
    void selectCountry();

private:
    QVBoxLayout* m_left_layout {nullptr};
    QPushButton* button {nullptr};
    QVBoxLayout* containerLayout {nullptr};
    QWidget* containerWidget {nullptr};
private:
    QSet<QPushButton*> clickedbuttons {nullptr};
    QMap<QPushButton*, QList<QPushButton*>> addedButtons;
    QStringList country_name {nullptr};
    QStringList country_city_name {nullptr};
private:
    QScrollArea* scroll {nullptr};
signals:
     void countrySelected(const QString& country, bool isCityAllowed);
};


