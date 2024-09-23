#ifndef LEFTLAYOUT_H
#define LEFTLAYOUT_H

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
#include <exception>
#include <iostream>
#include "Layouts.h"


class LeftLayout : public QVBoxLayout , public Layouts
{
      Q_OBJECT
public:
   explicit LeftLayout(QWidget* parent = nullptr);
public:
    virtual void createLayouts() override;
    void createButtonsLeftLayout();

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
};

#endif // LEFTLAYOUT_H
