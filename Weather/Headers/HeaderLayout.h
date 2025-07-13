#pragma once

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringListModel>
#include <QCompleter>
#include <memory>

#ifndef GOOGLE_PLACES_API_KEY
#define GOOGLE_PLACES_API_KEY "AIzaSyBcaW9EOU51kLZv2msprJ6efKfJ4ElK7Gg"
#endif

class HeaderLayout : public QHBoxLayout
{
    Q_OBJECT

public:
    explicit HeaderLayout(QWidget* parent = nullptr);
    void updateSearchPlaceholder(const QString& country, bool isCityAllowed);
    std::unique_ptr<QLineEdit> m_search;
    QPushButton* m_logoutBtn;

signals:
    void citySelected(const QString& text);
    void logoutRequested();

private slots:
    void onSearchTextEdited(const QString& text);
    void onAutocompleteDataReceived(QNetworkReply* reply);
    void onCityCompletionSelected(const QString& text);
    void onSearchReturnPressed();
    void onLogoutClicked();

private:
    QNetworkAccessManager* m_autocompleteNetworkManager;
    QStringListModel* m_completerModel;
    QCompleter* m_cityCompleter;
};
