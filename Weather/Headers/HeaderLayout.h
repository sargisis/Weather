#pragma once

#include <QHBoxLayout>
#include <QLineEdit>
#include <memory>
#include <QCompleter>        // Для автодополнения
#include <QStringListModel>  // Модель данных для QCompleter
#include <QNetworkAccessManager> // Для сетевых запросов к API автодополнения
#include <QNetworkReply>     // Для обработки ответов от сети

class HeaderLayout : public QHBoxLayout
{
    Q_OBJECT // Обязательно добавляем Q_OBJECT, так как будут слоты и сигналы

public:
    explicit HeaderLayout(QWidget* parent = nullptr);
    std::unique_ptr<QLineEdit> m_search;

    // Сигнал, который будет испускаться, когда город выбран или введен
signals:
    void citySelected(const QString& city);

public slots:
    void updateSearchPlaceholder(const QString& country, bool isCityAllowed);

private slots:
    // Слот для отправки запроса автодополнения при изменении текста
    void onSearchTextEdited(const QString& text);
    // Слот для обработки ответа от API автодополнения
    void onAutocompleteDataReceived(QNetworkReply* reply);
    // Слот для обработки выбора города из QCompleter
    void onCityCompletionSelected(const QString& text);
    // Слот для обработки нажатия Enter в поле поиска
    void onSearchReturnPressed();

private:
    QNetworkAccessManager* m_autocompleteNetworkManager; // Менеджер для запросов автодополнения
    QCompleter* m_cityCompleter; // Сам объект автодополнения
    QStringListModel* m_completerModel; // Модель, которая хранит список предложений

    // TODO: Замените на ваш реальный API-ключ Google Places API или другого сервиса
    const QString GOOGLE_PLACES_API_KEY = "AIzaSyC6Jtos6Qydb0SuwCo8AFUSwyNJ-H7Vkys"; // <-- ОБЯЗАТЕЛЬНО ЗАМЕНИТЬ
};
