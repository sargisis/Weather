#include <QApplication>
#include <QSettings>
#include "Auth.h"
#include "HomePage.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto* loginWindow = new AuthWindow();
    auto* mainApp = new HomePage();

    QSettings settings("MyCompany", "WeatherApp");
    const QString savedToken = settings.value("refresh_token").toString();
    settings.sync();

    if (!savedToken.isEmpty()) {
        // Есть токен — сразу запускаем приложение
        mainApp->show();
    } else {
        // Нет токена — логин
        QObject::connect(loginWindow, &AuthWindow::authSuccess, [&]() {
            loginWindow->close();
            mainApp->show();
        });

        loginWindow->show();
    }

    return app.exec();
}
