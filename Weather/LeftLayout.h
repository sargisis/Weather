#ifndef LEFTLAYOUT_H
#define LEFTLAYOUT_H

#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QtWidgets>

class LeftLayout : public QWidget
{
  Q_OBJECT
public:
    LeftLayout(QWidget* parent = nullptr);
public:
    void createButtons();
private:
    QPushButton* bt1 = new QPushButton;
    QPushButton* bt2 = new QPushButton;
    QPushButton* bt3 = new QPushButton;
    QVBoxLayout* main_layout = nullptr;

};

#endif // LEFTLAYOUT_H
