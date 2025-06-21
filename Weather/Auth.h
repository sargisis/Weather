#pragma once

#include <QLineEdit>
#include <QString>
#include <QPushButton>
#include <QVBoxLayout>

class AuthWindow : public QWidget {
private:
    Q_OBJECT
public:
    explicit AuthWindow(QWidget* parent = nullptr);
signals:
    void authSuccess();
};
