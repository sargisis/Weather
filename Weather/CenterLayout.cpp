#include "CenterLayout.h"

CenterLayout::CenterLayout(QWidget* parent)
    : QVBoxLayout{parent}, // Initialize the vertical layout with the specified parent
    m_networkManager{new QNetworkAccessManager(this)} // Create a network access manager for handling API requests
{
    m_center_layout = new QVBoxLayout; // Create a new vertical layout for the center area
    m_center_label = new QLabel("Weather Info"); // Create a label to display weather information

    m_center_label->setFixedSize(300, 600);

    m_center_label->setStyleSheet("background-color: yellow; color: blue"); // Set label's background and text color
    m_center_layout->addWidget(m_center_label); // Add the label to the center layout

    // Connect the finished signal of the network manager to the slot for handling received weather data
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &CenterLayout::onWeatherDataReceived);
}

void CenterLayout::createLayouts()
{
    addLayout(m_center_layout); // Add the center layout to the main layout
}

void CenterLayout::fetchWeatherData(const QString& latitude, const QString& longitude)
{
    QString apiKey = "716aa27058963431d3f2aafaeff2e033"; // Your OpenWeatherMap API key
    QString urlStr = QString("https://api.openweathermap.org/data/2.5/weather?lat=%1&lon=%2&appid=%3&units=metric&nocache=%4")
                         .arg(latitude) // Insert latitude into the URL
                         .arg(longitude) // Insert longitude into the URL
                         .arg(apiKey) // Insert API key into the URL
                         .arg(QDateTime::currentSecsSinceEpoch()); // Add a cache-busting parameter
    QUrl url(urlStr); // Create a URL object from the constructed URL string
    m_networkManager->get(QNetworkRequest(url)); // Send a GET request to the API
}

void CenterLayout::onWeatherDataReceived(QNetworkReply* reply)
{
    // Check for network errors in the reply
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll(); // Read the response data
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData); // Parse the JSON response
        QJsonObject jsonObj = jsonDoc.object(); // Convert the document to a JSON object

        // Extract temperature and weather description from the JSON object
        double temperature = jsonObj.value(QString("main")).toObject().value(QString("temp")).toDouble();
        QString weatherDescription = jsonObj.value(QString("weather")).toArray()[0].toObject().value(QString("description")).toString();
        QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss"); // Get the current time

        // Update the label with the fetched weather data
        m_center_label->setText(QString("Temperature: %1 °C\n%2\nUpdated at: %3")
                                    .arg(temperature)
                                    .arg(weatherDescription)
                                    .arg(currentTime));
    } else {
        // Display an error message if there was an issue retrieving the weather data
        m_center_label->setText("Error retrieving weather data: " + reply->errorString());
    }
    reply->deleteLater(); // Cleanup the reply object
}

void CenterLayout::fetchWeatherDataForCity(const QString& city)
{
    QString apiKey = "716aa27058963431d3f2aafaeff2e033"; // Your OpenWeatherMap API key
    QString urlStr = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric&nocache=%3")
                         .arg(city) // Insert city name into the URL
                         .arg(apiKey) // Insert API key into the URL
                         .arg(QDateTime::currentSecsSinceEpoch()); // Add a cache-busting parameter
    QUrl url(urlStr); // Create a URL object from the constructed URL string
    m_networkManager->get(QNetworkRequest(url)); // Send a GET request to the API
}
