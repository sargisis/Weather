#ifndef IFORGOTPASSWORD_H
#define IFORGOTPASSWORD_H

class IForgotPassword
{
public:
    virtual ~IForgotPassword() = default;
    virtual void forgotPasswordUser(const QString& email) = 0;
};

#endif // IFORGOTPASSWORD_H
