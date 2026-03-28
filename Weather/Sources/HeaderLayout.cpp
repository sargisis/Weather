#include "../Headers/HeaderLayout.h"
#include <QAction>
#include <QIcon>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QUrl>
#include <QDebug>
#include <QAbstractItemView>

HeaderLayout::HeaderLayout(QWidget* parent)
    : QHBoxLayout(parent),
    m_search(std::make_unique<QLineEdit>())
{
    m_search->setPlaceholderText("Search your city...");
    QAction* searchIcon = new QAction(QIcon(":/icons/search.png"), "", m_search.get());
    m_search->addAction(searchIcon, QLineEdit::LeadingPosition);

    m_search->setStyleSheet(R"(
        QLineEdit {
            background-color: #2b2b2b;
            border: 1px solid #444;
            border-radius: 12px;
            padding: 10px 40px 10px 15px;
            font-size: 17px;
            color: #e0e0e0;
            selection-background-color: #007BFF;
            selection-color: white;
        }
        QLineEdit:focus {
            border: 1px solid #0099ff;
            background-color: #333;
        }
        QLineEdit::placeholder {
            color: #999;
        }
    )");

    // m_logoutBtn = new QPushButton("Log Out");
    // m_logoutBtn->setStyleSheet(R"(
    //     QPushButton {
    //         background-color: #d9534f;
    //         color: white;
    //         border-radius: 8px;
    //         padding: 6px 12px;
    //         font-weight: bold;
    //         font-size: 13px;
    //     }
    //     QPushButton:hover {
    //         background-color: #c9302c;
    //     }
    // )");
    // m_logoutBtn->setFixedHeight(36);
    // m_logoutBtn->setCursor(Qt::PointingHandCursor); DELETE SOON

    // connect(m_logoutBtn, &QPushButton::clicked, this, &HeaderLayout::onLogoutClicked); DELETE SOON

    // 🔌 Сеть и модель
    m_autocompleteNetworkManager = new QNetworkAccessManager(this);
    m_completerModel = new QStringListModel(this);
    m_cityCompleter = new QCompleter(m_completerModel, this);
    m_cityCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    m_cityCompleter->setFilterMode(Qt::MatchContains);
    m_cityCompleter->setCompletionMode(QCompleter::PopupCompletion);
    m_search->setCompleter(m_cityCompleter);

    // Стили для всплывающих подсказок
    if (m_cityCompleter->popup()) {
        m_cityCompleter->popup()->setStyleSheet(R"(
            QAbstractItemView {
                background-color: #2c3e50;
                border: 1px solid #3498db;
                border-radius: 5px;
                padding: 3px;
                outline: none;
            }
            QListView::item {
                padding: 8px 12px;
                color: #ecf0f1;
                font-size: 15px;
                border-radius: 3px;
                margin-bottom: 1px;
            }
            QListView::item:selected {
                background-color: #3498db;
                color: white;
                font-weight: bold;
            }
            QListView::item:hover {
                background-color: #1abc9c;
                color: white;
            }
        )");
    }

    // Сигналы
    connect(m_search.get(), &QLineEdit::textEdited, this, &HeaderLayout::onSearchTextEdited);
    connect(m_autocompleteNetworkManager, &QNetworkAccessManager::finished, this, &HeaderLayout::onAutocompleteDataReceived);
    connect(m_cityCompleter, QOverload<const QString&>::of(&QCompleter::activated), this, &HeaderLayout::onCityCompletionSelected);
    connect(m_search.get(), &QLineEdit::returnPressed, this, &HeaderLayout::onSearchReturnPressed);

    // Кнопка избранного
    m_favoriteBtn = new QPushButton("☆");
    m_favoriteBtn->setStyleSheet("font-size: 24px; color: #ffcc00; background: transparent; border: none; outline: none;");
    m_favoriteBtn->setCursor(Qt::PointingHandCursor);
    m_favoriteBtn->setToolTip("Toggle Favorite");
    
    connect(m_favoriteBtn, &QPushButton::clicked, this, [this]() {
        emit favoriteToggled();
    });

    // В макет
    addStretch();
    addWidget(m_search.get(), 1);
    addWidget(m_favoriteBtn); // Добавляем справа от поиска
    addStretch();
    setContentsMargins(10, 10, 10, 10);
}

void HeaderLayout::setFavoriteState(bool isFavorite)
{
    m_isCurrentCityFavorite = isFavorite;
    m_favoriteBtn->setText(isFavorite ? "⭐" : "☆");
}

void HeaderLayout::updateSearchPlaceholder(const QString& country, bool isCityAllowed)
{
    m_search->setPlaceholderText(isCityAllowed ? "Search your city..." : QString("Search in %1...").arg(country));
}

void HeaderLayout::onSearchTextEdited(const QString& text)
{
    if (text.length() < 3) {
        m_completerModel->setStringList(QStringList());
        return;
    }

    QUrl url("https://maps.googleapis.com/maps/api/place/autocomplete/json");
    QUrlQuery query;
    query.addQueryItem("input", text);
    query.addQueryItem("types", "(cities)");
    query.addQueryItem("language", "en");
    query.addQueryItem("key", GOOGLE_PLACES_API_KEY);
    url.setQuery(query);

    m_autocompleteNetworkManager->get(QNetworkRequest(url));
}

void HeaderLayout::onAutocompleteDataReceived(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Autocomplete API Error:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data);
    QJsonObject jsonObject = jsonResponse.object();

    QJsonArray predictions = jsonObject["predictions"].toArray();
    QStringList suggestions;
    for (const QJsonValue& value : predictions) {
        suggestions.append(value.toObject()["description"].toString());
    }


    m_completerModel->setStringList(suggestions);
    reply->deleteLater();
}

void HeaderLayout::onCityCompletionSelected(const QString& text)
{
    emit citySelected(text.split(",").first().trimmed());
}

void HeaderLayout::onSearchReturnPressed()
{
    QString enteredText = m_search->text().trimmed();
    if (!enteredText.isEmpty()) {
        emit citySelected(enteredText.split(",").first().trimmed());
    }
}

// void HeaderLayout::onLogoutClicked() // DELETE SOON
// {
//     emit logoutRequested();
// }
