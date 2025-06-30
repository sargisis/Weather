#ifndef ILOGINWINDOW_H
#define ILOGINWINDOW_H


class ILoginWindow
{
public:
    virtual ~ILoginWindow() = default;
    virtual void LogInUser(const QString& email , const QString& passowrd) = 0;
};

#endif // ILOGINWINDOW_H
