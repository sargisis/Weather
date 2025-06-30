#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

class GoogleAuthServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit GoogleAuthServer(QObject* parent = nullptr);
    void startListening();

signals:
    void receivedAuthCode(const QString& code);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};
