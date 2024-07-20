#ifndef LEFTLAYOUT_H
#define LEFTLAYOUT_H

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QString>
#include "Layouts.h"


class LeftLayout : public QVBoxLayout , public Layouts
{
      Q_OBJECT
public:
   explicit LeftLayout(QWidget* parent = nullptr);
public:
   virtual void createLayouts() override;
    void createButtonsLeftLayout();
private:
    QVBoxLayout* m_left_layout {nullptr};


    QPushButton* button{nullptr};

    QVBoxLayout* containerLayout{nullptr};
    QWidget* containerWidget{nullptr};
};

#endif // LEFTLAYOUT_H
