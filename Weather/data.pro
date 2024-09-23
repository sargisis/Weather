# Название проекта
TEMPLATE = app
TARGET = WeatherApp

# Используемые языки
QT += core gui widgets multimedia network

CONFIG += c++17

SOURCES += \
    main.cpp \
    WeatherApp.cpp \
    LeftLayout.cpp

HEADERS += \
    WeatherApp.h \
    LeftLayout.h

FORMS += \
    WeatherApp.ui

RESOURCES += resources.qrc

INCLUDEPATH += .
