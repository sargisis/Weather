#include <QApplication>
#include "Auth.h"
#include "HomePage.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto* loginWindow = new AuthWindow();
    auto* mainApp = new HomePage();

    QObject::connect(loginWindow, &AuthWindow::authSuccess, [&]() {
        loginWindow->hide();
        mainApp->show();
    });

    QObject::connect(mainApp, &HomePage::requestLogout, [&]() {
        mainApp->hide();
        loginWindow->show();
    });

    loginWindow->show();
    return app.exec();
}
