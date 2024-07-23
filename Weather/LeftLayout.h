#ifndef LEFTLAYOUT_H
#define LEFTLAYOUT_H

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QString>
#include <QSet>
#include <QMap>
#include <QList>

#include "Layouts.h"
#include <cstddef>


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
private:
    QVBoxLayout* m_left_layout {nullptr};
    QPushButton* button{nullptr};
    QVBoxLayout* containerLayout{nullptr};
    QWidget* containerWidget{nullptr};
    bool is_created = false;
    QSet<QPushButton*> clickedbuttons;
    QMap<QPushButton*, QList<QPushButton*>> addedButtons;
};

#endif // LEFTLAYOUT_H
