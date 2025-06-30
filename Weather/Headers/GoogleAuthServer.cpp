#include "../Headers/GoogleAuthServer.h"
#include <QUrlQuery>
#include <QUrl>
#include <QDebug>

GoogleAuthServer::GoogleAuthServer(QObject* parent)
    : QTcpServer(parent) {}

void GoogleAuthServer::startListening() {
    listen(QHostAddress::Any, 8080);
}

void GoogleAuthServer::incomingConnection(qintptr socketDescriptor) {
    auto* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead, this, [=]() {
        QByteArray data = socket->readAll();
        QString request(data);

        QRegularExpression re("GET /\\?code=([^\\s&]+)");
        QRegularExpressionMatch match = re.match(request);

        QString htmlResponse = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: text/html\r\n\r\n"
                               "<html><body><h1>Google login successful. You may close this window.</h1></body></html>";

        socket->write(htmlResponse.toUtf8());
        socket->disconnectFromHost();

        if (match.hasMatch()) {
            QString code = match.captured(1);
            emit receivedAuthCode(code);
        }
    });

    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}
