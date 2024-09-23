#ifndef HEADERLAYOUT_H
#define HEADERLAYOUT_H

#include <QInputDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>

class HeaderLayout : public QHBoxLayout
{
public:
    HeaderLayout(QWidget* parent = nullptr);

    void createInputDialogInHeaderLayout();

private:
    QLabel* m_header_label;
};

#endif // HEADER_LAYOUT_H
