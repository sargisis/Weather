#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QtWidgets>
#include <QLabel>

class HomePage : public QWidget
{
    Q_OBJECT
public:
    HomePage(QWidget* parent = nullptr);
public:
    void createLayout();
private:
    QGridLayout* main_layout = nullptr;
    QHBoxLayout* header_layout = nullptr;
    QLabel* header_label = nullptr;

    QVBoxLayout* left_layout = nullptr;
    QLabel* left_label = nullptr;

    QVBoxLayout* right_layout = nullptr;
    QLabel* right_label = nullptr;
};

#endif // HOMEPAGE_H
