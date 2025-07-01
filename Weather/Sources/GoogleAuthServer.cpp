#include "../Headers/../Headers/GoogleAuthServer.h" // Включаем заголовочный файл для нашего класса
#include <QUrlQuery> // Используется для парсинга параметров URL (хотя в текущем коде регулярное выражение)
#include <QUrl>      // Для работы с URL (не используется напрямую для парсинга запроса здесь)
#include <QDebug>    // Для вывода отладочных сообщений

// Конструктор класса GoogleAuthServer.
// Инициализирует базовый класс QTcpServer.
GoogleAuthServer::GoogleAuthServer(QObject* parent)
    : QTcpServer(parent) {}

// Метод для запуска сервера.
// Сервер начинает прослушивать все доступные сетевые интерфейсы (QHostAddress::Any)
// на порту 8080.
void GoogleAuthServer::startListening() {
    listen(QHostAddress::Any, 8080);
    qDebug() << "Google Auth Server listening on port 8080..."; // Отладочное сообщение о запуске
}

// Переопределенный метод, вызываемый при каждом новом входящем соединении.
// `socketDescriptor` - это низкоуровневый идентификатор сокета.
void GoogleAuthServer::incomingConnection(qintptr socketDescriptor) {
    // Создаем новый QTcpSocket для обработки текущего соединения.
    // Устанавливаем `this` (GoogleAuthServer) как родителя, чтобы сокет автоматически удалился
    // при уничтожении сервера, если он не был явно удален раньше.
    auto* socket = new QTcpSocket(this);
    // Присваиваем сокету его дескриптор.
    socket->setSocketDescriptor(socketDescriptor);

    // Подключаем сигнал readyRead сокета к лямбда-функции.
    // readyRead испускается, когда есть новые данные для чтения из сокета.
    connect(socket, &QTcpSocket::readyRead, this, [=]() {
        QByteArray data = socket->readAll(); // Читаем все доступные данные из сокета.
        QString request(data);               // Преобразуем данные в строку для удобства парсинга.

        qDebug() << "Received request:\n" << request; // Отладочный вывод полученного запроса

        // Используем регулярное выражение для поиска кода авторизации в HTTP GET-запросе.
        // Ожидаем запрос вида "GET /?code=YOUR_CODE_HERE..."
        QRegularExpression re("GET /\\?code=([^\\s&]+)");
        QRegularExpressionMatch match = re.match(request); // Выполняем сопоставление.

        // Формируем простой HTML-ответ для клиента (браузера).
        // Этот ответ сообщает пользователю, что авторизация прошла успешно и окно можно закрыть.
        QString htmlResponse = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: text/html\r\n\r\n"
                               "<html><body><h1>Google login successful. You may close this window.</h1></body></html>";

        socket->write(htmlResponse.toUtf8()); // Отправляем HTTP-ответ клиенту.
        socket->disconnectFromHost();         // Отключаем сокет от хоста.

        // Проверяем, было ли найдено совпадение с регулярным выражением (т.е. был ли найден 'code').
        if (match.hasMatch()) {
            QString code = match.captured(1); // Извлекаем захваченную группу (сам код авторизации).
            qDebug() << "Received authorization code:" << code; // Отладочный вывод найденного кода.
            emit receivedAuthCode(code); // Испускаем сигнал, передавая полученный код.
        } else {
            qWarning() << "No authorization code found in the request."; // Предупреждение, если код не найден
        }
    });

    // Подключаем сигнал disconnected сокета к слоту deleteLater.
    // Это гарантирует, что QTcpSocket будет автоматически удален из памяти,
    // когда соединение будет разорвано, предотвращая утечки памяти.
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}
