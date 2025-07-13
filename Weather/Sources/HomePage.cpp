#include "../Headers/HomePage.h" // Включаем заголовочный файл для класса HomePage
#include <QScreen>          // Для получения информации о экране (для центрирования окна)
#include <QGuiApplication>  // Для доступа к QApplication (для получения первичного экрана)
#include <QVBoxLayout>      // Для вертикального расположения виджетов (используется для оберток)
#include <QGridLayout>      // Для основного макета окна (сетка)
#include <QDebug>           // Для вывода отладочных сообщений

// Конструктор класса HomePage
HomePage::HomePage(QWidget* parent)
    : QWidget(parent) // Вызываем конструктор базового класса QWidget
{
    // Настройка основного окна приложения
    setWindowTitle("Weather-App"); // Устанавливаем заголовок окна.
    resize(900, 750);             // Устанавливаем начальный размер окна.
    setMinimumSize(800, 600);     // Устанавливаем минимальный размер окна, ниже которого оно не может быть уменьшено.

    // Применяем CSS-стили для всего окна и его дочерних виджетов.
    setStyleSheet(R"(
        QWidget { background-color: #1c1c1c; color: #ffffff; } /* Общие стили для всех виджетов */
        QLineEdit {
            background-color: #2a2a2a; border: 2px solid #3e3e3e;
            border-radius: 10px; padding: 10px; color: #ffffff; font-size: 16px;
        }
        QPushButton {
            background-color: #007BFF; color: white; font-weight: bold;
            border-radius: 10px; padding: 10px 20px; font-size: 16px;
        }
        QPushButton:hover { background-color: #0056b3; } /* Стиль при наведении на кнопку */
        QLabel { font-size: 16px; color: #ffffff; } /* Общие стили для меток */
    )");

    createLayout(); // Вызываем метод для создания и настройки всех элементов интерфейса.

    // Центрирование окна на экране
    QScreen* screen = QGuiApplication::primaryScreen(); // Получаем указатель на основной экран.
    QRect screenGeometry = screen->geometry();         // Получаем геометрию (размеры и положение) экрана.
    // Вычисляем X-координату для центрирования окна.
    int x = (screenGeometry.width() - this->width()) / 2;
    // Вычисляем Y-координату для центрирования окна.
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y); // Перемещаем окно в вычисленные координаты.
}

// Метод для создания и настройки всех макетов и виджетов на главной странице.
void HomePage::createLayout()
{
    // Инициализация основного макета (сетки).
    main_layout = std::make_unique<QGridLayout>();

    // Инициализация уникальных указателей для всех под-макетов.
    // Они будут автоматически удалены при уничтожении HomePage.
    m_header_layout = std::make_unique<HeaderLayout>();
    m_center_layout = std::make_unique<CenterLayout>();
    m_right_layout = std::make_unique<RightLayout>();

    // Создание виджетов-оберток для CenterLayout и RightLayout.
    // Это позволяет добавлять макеты в QGridLayout, который работает с QWidget*.
    centerWidget = new QWidget(this); // Создаем виджет-контейнер для центрального макета.
    {
        auto* wrapperLayout = new QVBoxLayout(centerWidget); // Создаем вертикальный макет внутри обертки.
        wrapperLayout->addLayout(m_center_layout.get());    // Добавляем CenterLayout в эту обертку.
        wrapperLayout->setContentsMargins(0, 0, 0, 0);       // Убираем отступы обертки.
    }

    rightWidget = new QWidget(this); // Создаем виджет-контейнер для правого макета.
    {
        auto* wrapperLayout = new QVBoxLayout(rightWidget); // Создаем вертикальный макет внутри обертки.
        wrapperLayout->addLayout(m_right_layout.get());     // Добавляем RightLayout в эту обертку.
        wrapperLayout->setContentsMargins(0, 0, 0, 0);        // Убираем отступы обертки.
    }

    // --- Подключение поиска и обновления данных ---
    // Подключаем сигнал returnPressed (нажатие Enter) из поля поиска HeaderLayout
    // к лямбда-функции, которая будет запрашивать данные о погоде.
    connect(m_header_layout->m_search.get(), &QLineEdit::returnPressed, this, [this]() {
        // Получаем текст из поля поиска, удаляя пробелы по краям.
        const QString city = m_header_layout->m_search->text().trimmed();
        if (!city.isEmpty()) { // Если город введен (не пустая строка)
            // Запрашиваем текущие погодные данные для города в CenterLayout.
            m_center_layout->fetchWeatherDataForCity(city);
            // Запрашиваем прогноз погоды для города в RightLayout.
            m_right_layout->fetchFutureWeather(city);
        }
    });

    connect(m_header_layout.get(), &HeaderLayout::logoutRequested, this, [this]() {
        QSettings settings("MyCompany", "WeatherApp");
        settings.remove("refresh_token");
        emit requestLogout();
    });

    // Добавление макетов и виджетов в основной QGridLayout.
    // m_header_layout занимает всю верхнюю строку (строка 0, столбец 0, растягиваясь на 1 строку и 2 столбца).
    main_layout->addLayout(m_header_layout.get(), 0, 0, 1, 2);
    // centerWidget (с CenterLayout внутри) располагается в строке 1, столбце 0.
    main_layout->addWidget(centerWidget, 1, 0);
    // rightWidget (с RightLayout внутри) располагается в строке 1, столбце 1.
    main_layout->addWidget(rightWidget, 1, 1);

    // Настройка растяжения столбцов в QGridLayout.
    // Столбец 0 (с CenterLayout) будет занимать 3 части от доступного пространства.
    main_layout->setColumnStretch(0, 3);
    // Столбец 1 (с RightLayout) будет занимать 2 части от доступного пространства.
    main_layout->setColumnStretch(1, 2);
    // Устанавливаем отступы для основного макета.
    main_layout->setContentsMargins(20, 20, 20, 20);
    // Устанавливаем горизонтальный отступ между столбцами.
    main_layout->setHorizontalSpacing(15);
    // Устанавливаем вертикальный отступ между строками.
    main_layout->setVerticalSpacing(10);

    // Устанавливаем созданный QGridLayout как основной макет для HomePage.
    setLayout(main_layout.get());
}


