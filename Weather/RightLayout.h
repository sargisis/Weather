#ifndef RIGHTLAYOUT_H
#define RIGHTLAYOUT_H

#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>

#include "Layouts.h"


class RightLayout : public QVBoxLayout , public Layouts
{
public:
  explicit RightLayout(QWidget* parent = nullptr);
public:
  virtual void createLayouts() override;
private:
  QVBoxLayout* m_right_layout {nullptr};
  QLabel* m_right_label {nullptr};
};

#endif // RIGHTLAYOUT_H
