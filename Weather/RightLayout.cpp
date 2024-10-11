#include "RightLayout.h"

RightLayout::RightLayout(QWidget* parent)
    : QVBoxLayout(parent)
    , m_network_manager{new QNetworkAccessManager(this)} // Initialize the network manager for handling API requests
{
    // Initialize the label to display future weather info
    m_right_label = new QLabel("Future Weather info");
    m_right_label->setStyleSheet("background-color: blue; color: white"); // Set the label's style

    // Connect the finished signal of the network manager to the handleNetworkInfo slot
    QObject::connect(m_network_manager, &QNetworkAccessManager::finished, this, &RightLayout::handleNetworkInfo);

    // Initialize your API key here
    m_apiKey = "716aa27058963431d3f2aafaeff2e033"; // Replace with your actual API key
}

void RightLayout::createLayouts()
{
    // Add the label to the layout
    addWidget(m_right_label);
}

void RightLayout::fetchFutureWeather(const QString& city)
{
    // Construct the API request URL using the city and API key for future weather
    QString forecastUrl = QString("http://api.openweathermap.org/data/2.5/forecast?q=%1&appid=%2&units=metric")
                              .arg(city) // Substitute the city name into the URL
                              .arg(m_apiKey); // Substitute the API key into the URL
    QUrl url(forecastUrl); // Create a QUrl object with the constructed URL

    // Make a GET request to fetch the future weather information
    m_network_manager->get(QNetworkRequest(url));
}

void RightLayout::handleNetworkInfo(QNetworkReply* reply)
{
    // Check if there was a network error
    if (reply->error() != QNetworkReply::NoError) {
        m_right_label->setText("Error fetching weather data."); // Display error message
        reply->deleteLater(); // Cleanup the reply object
        return; // Exit the function if there was an error
    }

    // Read the response data from the reply
    QByteArray response = reply->readAll();

    // Parse the response as JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
    QJsonObject jsonObject = jsonDoc.object(); // Convert the JSON document to a JSON object

    // Extract the list of forecasts from the JSON object
    QJsonArray forecastArray = jsonObject["list"].toArray();

    QString forecastText; // Variable to hold formatted forecast text

    // Extract and format the first 5 future weather forecasts
    for (int i = 0; i < 5 && i < forecastArray.size(); ++i) {
        QJsonObject forecastObject = forecastArray[i].toObject(); // Get the forecast object
        QJsonObject mainObject = forecastObject["main"].toObject(); // Get the main weather data
        double temperature = mainObject["temp"].toDouble(); // Extract the temperature

        QJsonArray weatherArray = forecastObject["weather"].toArray(); // Get the weather array
        QString description = weatherArray[0].toObject()["description"].toString(); // Extract the weather description

        QString dateTime = forecastObject["dt_txt"].toString(); // Get the forecast date and time

        // Format and append the weather data for display
        forecastText.append(QString("%1: %2°C, %3\n").arg(dateTime).arg(temperature).arg(description));
    }

    // Update the label with the formatted weather forecast text
    m_right_label->setText(forecastText);

    // Cleanup the reply object
    reply->deleteLater();
}
