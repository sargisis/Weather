#ifndef IREGISTERWINDOW_H
#define IREGISTERWINDOW_H


class IRegisterWindow
{
public:
    virtual ~IRegisterWindow() = default;
    virtual void RegisterUser(const QString& email , const QString& passowrd) = 0;
};

#endif // IREGISTERWINDOW_H
