#pragma once

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringListModel>
#include <QCompleter>
#include <memory>

// Ключ API для Google Places Autocomplete
#define GOOGLE_PLACES_API_KEY "AIzaSyBu0eQNuDh8Tml1gw5gY46evZs3PSSwxo0"

// HeaderLayout — горизонтальный лейаут, содержащий поле поиска с автодополнением.
// Выполняет запросы к Google Places API для получения подсказок по городам.
class HeaderLayout : public QHBoxLayout
{
    Q_OBJECT

public:
    explicit HeaderLayout(QWidget* parent = nullptr);

    // Обновляет placeholder поля поиска в зависимости от страны и доступности городов
    void updateSearchPlaceholder(const QString& country, bool isCityAllowed);

    // Поле ввода для поиска города
    std::unique_ptr<QLineEdit> m_search;

signals:
    // Сигнал испускается при выборе города пользователем
    void citySelected(const QString& text);

private slots:
    // Обработчик изменения текста в поиске
    void onSearchTextEdited(const QString& text);

    // Обработчик ответа от Google Places API
    void onAutocompleteDataReceived(QNetworkReply* reply);

    // Обработчик выбора города из подсказок
    void onCityCompletionSelected(const QString& text);

    // Обработчик нажатия Enter в поле поиска
    void onSearchReturnPressed();

private:
    QNetworkAccessManager* m_autocompleteNetworkManager = nullptr;
    QStringListModel* m_completerModel = nullptr;
    QCompleter* m_cityCompleter = nullptr;
};
