#pragma once

#include <QTcpServer>          // Базовый класс для создания TCP-серверов
#include <QTcpSocket>          // Класс для обмена данными по TCP-соединению
#include <QString>             // Для работы со строками
#include <QRegularExpression>  // Для работы с регулярными выражениями (поиск кода авторизации)
#include <QRegularExpressionMatch> // Для результатов сопоставления регулярных выражений

// Класс GoogleAuthServer наследуется от QTcpServer,
// предназначен для прослушивания входящих HTTP-запросов (конкретно - ответа от Google Auth).
class GoogleAuthServer : public QTcpServer
{
    Q_OBJECT // Макрос, необходимый для использования сигналов, слотов и других мета-объектных возможностей Qt

public:
    // Конструктор класса. Принимает родительский объект QObject.
    explicit GoogleAuthServer(QObject* parent = nullptr);

    // Метод для запуска сервера в режиме прослушивания входящих соединений.
    void startListening();

signals:
    // Сигнал, который испускается, когда сервер успешно получает код авторизации от Google.
    // Код передается в виде строки.
    void receivedAuthCode(const QString& code);

protected:
    // Переопределенный виртуальный метод из QTcpServer.
    // Вызывается автоматически, когда появляется новое входящее соединение.
    // `socketDescriptor` - это дескриптор сокета, который можно использовать для создания QTcpSocket.
    void incomingConnection(qintptr socketDescriptor) override;
};
