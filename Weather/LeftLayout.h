#ifndef LEFTLAYOUT_H
#define LEFTLAYOUT_H

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>


class LeftLayout : public QVBoxLayout
{
public:
   explicit LeftLayout(QWidget* parent = nullptr);
public:
    void createLeftLayout();
private:
    QVBoxLayout* m_left_layout {nullptr};
    QLabel* m_left_label {nullptr};
};

#endif // LEFTLAYOUT_H
