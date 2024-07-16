#ifndef RIGHTLAYOUT_H
#define RIGHTLAYOUT_H

#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>


class RightLayout : public QVBoxLayout
{
public:
  explicit RightLayout(QWidget* parent = nullptr);
public:
  void createRightLayout();
private:
  QVBoxLayout* m_right_layout {nullptr};
  QLabel* m_right_label {nullptr};
};

#endif // RIGHTLAYOUT_H
