#ifndef IWINDOWFACTORY_H
#define IWINDOWFACTORY_H

#include <QWidget>
#include <QString>

class IWindowFactory
{
public:
    virtual ~IWindowFactory() = default;

    virtual QWidget* createLoginWindow(QWidget* parent = nullptr) = 0;
    virtual QWidget* createRegisterWindow(QWidget* parent = nullptr) = 0;
    virtual QWidget* createForgotPasswordWindow(QWidget* parent = nullptr) = 0;
};

#endif // IWINDOWFACTORY_H
