#ifndef CENTERLAYOUT_H
#define CENTERLAYOUT_H

#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include "Layouts.h"

class CenterLayout : public QVBoxLayout, public Layouts
{
public:
   explicit CenterLayout(QWidget* parent = nullptr);
public:
   virtual void createLayouts() override;
private:
   QVBoxLayout* m_center_layout;
   QLabel* m_center_label;

};

#endif // CENTERLAYOUT_H
