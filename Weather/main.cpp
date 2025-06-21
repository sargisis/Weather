#include <QApplication>
#include "Auth.h"
#include "HomePage.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    HomePage mainApp;
    // AuthWindow loginWindow;

    // QObject::connect(&loginWindow, &AuthWindow::authSuccess, [&]() {

    // });
    mainApp.show();
    // loginWindow.show();
    return app.exec();
}
