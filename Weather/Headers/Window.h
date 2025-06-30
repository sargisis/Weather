#ifndef WINDOW_FACTORY_H
#define WINDOW_FACTORY_H

#include "./Interfaces/IWindowFactory.h"
#include "Register.h"
#include "Login.h"
#include "ForgotPassowrd.h"

class WindowFactory : public IWindowFactory
{
public:
    QWidget* createLogInWindow(QWidget* parent = nullptr) {
        return new LogINWindow(parent);
    }

    QWidget* createRegisterWindow(QWidget* parent = nullptr) {
        return new RegisterWindow(parent);
    }

    QWidget* createForgotPassoworddWindow(QWidget* parent = nullptr) {
        return new ForgotPasswordWindow(parent);
    }

};

#endif // WINDOW_H
