#include "../Headers/HomePage.h"
#include <QScreen>
#include <QGuiApplication>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

HomePage::HomePage(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Weather-App");
    resize(900, 750);
    setMinimumSize(800, 600);

    setStyleSheet(R"(
        QWidget { background-color: #1c1c1c; color: #ffffff; }
        QLineEdit {
            background-color: #2a2a2a; border: 2px solid #3e3e3e;
            border-radius: 10px; padding: 10px; color: #ffffff; font-size: 16px;
        }
        QPushButton {
            background-color: #007BFF; color: white; font-weight: bold;
            border-radius: 10px; padding: 10px 20px; font-size: 16px;
        }
        QPushButton:hover { background-color: #0056b3; }
        QLabel { font-size: 16px; color: #ffffff; }
    )");

    createLayout();

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);

    // Автоопределение локации по IP и подгрузка погоды при запуске
    auto* ipManager = new QNetworkAccessManager(this);
    QNetworkReply* reply = ipManager->get(QNetworkRequest(QUrl("http://ip-api.com/json/")));
    connect(reply, &QNetworkReply::finished, this, [this, reply, ipManager]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QString city = doc.object()["city"].toString();
            if (!city.isEmpty()) {
                m_header_layout->m_search->setText(city); // Ставим город в форму поиска
                m_center_layout->fetchWeatherDataForCity(city); // Текущая погода
                m_right_layout->fetchFutureWeather(city); // График/Прогноз погоды
            }
        } else {
            qWarning() << "Failed to detect IP location:" << reply->errorString();
        }
        reply->deleteLater();
        ipManager->deleteLater();
    });
}

void HomePage::createLayout()
{
    main_layout = std::make_unique<QGridLayout>();
    m_header_layout = std::make_unique<HeaderLayout>();
    m_center_layout = std::make_unique<CenterLayout>();
    m_right_layout = std::make_unique<RightLayout>();
    m_navigation_layout = std::make_unique<NavigationLayout>();

    centerWidget = new QWidget(this);
    {
        auto* wrapperLayout = new QVBoxLayout(centerWidget);
        wrapperLayout->addLayout(m_center_layout.get());
        wrapperLayout->setContentsMargins(0, 0, 0, 0);
    }

    rightWidget = new QWidget(this);
    {
        auto* wrapperLayout = new QVBoxLayout(rightWidget);
        wrapperLayout->addLayout(m_right_layout.get());
        wrapperLayout->setContentsMargins(0, 0, 0, 0);
    }

    navigationWidget = new QWidget(this);
    {
        auto* wrapperLayout = new QVBoxLayout(navigationWidget);
        wrapperLayout->addLayout(m_navigation_layout.get());
        wrapperLayout->setContentsMargins(0, 0, 0, 0);
    }

    connect(m_header_layout->m_search.get(), &QLineEdit::returnPressed, this, [this]() {
        const QString city = m_header_layout->m_search->text().trimmed();
        if (!city.isEmpty()) {
            m_center_layout->fetchWeatherDataForCity(city);
            m_right_layout->fetchFutureWeather(city);
            checkIfFavorite(city);
        }
    });

    // Также реагируем на выбор города мышкой из выпадающего списка
    connect(m_header_layout.get(), &HeaderLayout::citySelected, this, [this](const QString& city) {
        if (!city.isEmpty()) {
            m_center_layout->fetchWeatherDataForCity(city);
            m_right_layout->fetchFutureWeather(city);
            m_header_layout->m_search->setText(city);
            checkIfFavorite(city);
        }
    });

    // Избранное
    connect(m_header_layout.get(), &HeaderLayout::favoriteToggled, this, &HomePage::toggleFavorite);
    connect(m_navigation_layout.get(), &NavigationLayout::favoriteCityClicked, this, [this](const QString& city) {
        if (!city.isEmpty()) {
            m_center_layout->fetchWeatherDataForCity(city);
            m_right_layout->fetchFutureWeather(city);
            m_header_layout->m_search->setText(city);
            checkIfFavorite(city);
        }
    });

    connect(m_navigation_layout.get(), &NavigationLayout::weatherClicked, this, [this]() {
        m_stackedWidget->setCurrentWidget(m_dashboardPage);
    });

    connect(m_navigation_layout.get(), &NavigationLayout::mapClicked, this, [this]() {
        m_stackedWidget->setCurrentWidget(m_mapPage);
    });

    connect(m_right_layout.get(), &RightLayout::coordinatesFetched, this, [this](double lat, double lon) {
        if (m_mapPage) {
            m_mapPage->setCenter(lat, lon, 12);
        }
    });

    // Навигация (слева) занимает обе строки: 0 и 1
    main_layout->addWidget(navigationWidget, 0, 0, 2, 1);
    
    // Верхний поиск (занимает колонки 1 и 2 в нулевой строке)
    main_layout->addLayout(m_header_layout.get(), 0, 1, 1, 2);
    
    // Создаем Stacked Widget
    m_stackedWidget = std::make_unique<QStackedWidget>();

    // 1. Dashboard Page
    m_dashboardPage = new QWidget();
    QGridLayout* dashLayout = new QGridLayout(m_dashboardPage);
    dashLayout->addWidget(centerWidget, 0, 0);
    dashLayout->addWidget(rightWidget, 0, 1);
    dashLayout->setColumnStretch(0, 3);
    dashLayout->setColumnStretch(1, 2);
    dashLayout->setContentsMargins(0, 0, 0, 0);
    dashLayout->setHorizontalSpacing(15);
    dashLayout->setVerticalSpacing(10);
    
    // 2. Map Page - Наш кастомный движок карт!
    m_mapPage = new MapWidget(this);

    // ----- NavigationEngine: подключаем к карте -----
    m_navEngine = new NavigationEngine(this);

    // Новый маршрут от OSRM → передаём в MapWidget для отрисовки
    connect(m_navEngine, &NavigationEngine::routeUpdated, m_mapPage, &MapWidget::setRoute);

    // GPS позиция → обновляем синюю точку на карте
    connect(m_navEngine, &NavigationEngine::positionChanged, m_mapPage, &MapWidget::updateGpsPosition);

    // Смена шага манёвра → центрируем на текущей позиции
    connect(m_navEngine, &NavigationEngine::currentStepChanged, this, [this](RouteStep step) {
        if (m_mapPage) {
            m_mapPage->setCenter(step.coordinate.latitude(), step.coordinate.longitude(), 15);
        }
    });

    // 3. Settings Page
    m_settingsPage = new QWidget();
    QVBoxLayout* settingsLayout = new QVBoxLayout(m_settingsPage);
    settingsLayout->setAlignment(Qt::AlignTop);

    QLabel* settingsTitle = new QLabel("Settings");
    settingsTitle->setStyleSheet("font-size: 32px; font-weight: bold; color: #ffffff; margin-bottom: 20px;");
    settingsLayout->addWidget(settingsTitle);

    // Logout button
    QPushButton* logoutBtn = new QPushButton("Log Out");
    logoutBtn->setStyleSheet("background-color: #d9534f; color: white; padding: 10px 20px; font-size: 16px; font-weight: bold; border-radius: 8px; max-width: 200px;");
    logoutBtn->setCursor(Qt::PointingHandCursor);
    settingsLayout->addWidget(logoutBtn);

    connect(logoutBtn, &QPushButton::clicked, this, [this]() {
        QSettings settings("MyCompany", "WeatherApp");
        settings.remove("refresh_token");
        
        // Перезапуск приложения для возврата на экран логина
        qApp->quit(); 
    });

    settingsLayout->addSpacing(15);

    // Clear Favorites button
    QPushButton* clearFavBtn = new QPushButton("Clear Favorites");
    clearFavBtn->setStyleSheet("background-color: #f0ad4e; color: white; padding: 10px 20px; font-size: 16px; font-weight: bold; border-radius: 8px; max-width: 200px;");
    clearFavBtn->setCursor(Qt::PointingHandCursor);
    settingsLayout->addWidget(clearFavBtn);
    
    connect(clearFavBtn, &QPushButton::clicked, this, [this]() {
        QSettings settings("MyCompany", "WeatherApp");
        settings.remove("favorites");
        m_navigation_layout->updateFavorites(QStringList());
        m_header_layout->setFavoriteState(false);
    });

    m_stackedWidget->addWidget(m_dashboardPage);
    m_stackedWidget->addWidget(m_mapPage);
    m_stackedWidget->addWidget(m_settingsPage);

    // Обработка кнопки Settings из бокового меню
    connect(m_navigation_layout.get(), &NavigationLayout::profileClicked, this, [this]() {
        m_stackedWidget->setCurrentWidget(m_settingsPage);
    });

    // Добавляем StackedWidget на главный экран
    main_layout->addWidget(m_stackedWidget.get(), 1, 1, 1, 2);

    main_layout->setColumnStretch(0, 1);
    main_layout->setColumnStretch(1, 5); // 5 = 3 + 2 (column stretch of dashboard)
    main_layout->setContentsMargins(20, 20, 20, 20);
    main_layout->setHorizontalSpacing(15);
    main_layout->setVerticalSpacing(10);

    setLayout(main_layout.get());

    loadFavorites();
}

void HomePage::loadFavorites()
{
    QSettings settings("MyCompany", "WeatherApp");
    QStringList favorites = settings.value("favorites").toStringList();
    m_navigation_layout->updateFavorites(favorites);
}

void HomePage::checkIfFavorite(const QString& city)
{
    QSettings settings("MyCompany", "WeatherApp");
    QStringList favorites = settings.value("favorites").toStringList();
    m_header_layout->setFavoriteState(favorites.contains(city));
}

void HomePage::toggleFavorite()
{
    QString city = m_header_layout->m_search->text().trimmed();
    if (city.isEmpty()) return;

    QSettings settings("MyCompany", "WeatherApp");
    QStringList favorites = settings.value("favorites").toStringList();

    if (favorites.contains(city)) {
        favorites.removeAll(city);
        m_header_layout->setFavoriteState(false);
    } else {
        favorites.append(city);
        m_header_layout->setFavoriteState(true);
    }

    settings.setValue("favorites", favorites);
    m_navigation_layout->updateFavorites(favorites);
}
