cmake_minimum_required(VERSION 3.10)

project(Weather VERSION 0.1 LANGUAGES CXX)

# Enable Qt features
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Use modern C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

# Include headers
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers/layouts
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers/interfaces
)

# Project sources
set(PROJECT_SOURCES
    main.cpp

    # Layouts
    Sources/layouts/LeftLayout.cpp
    Sources/layouts/RightLayout.cpp
    Sources/layouts/CenterLayout.cpp
    Sources/Layouts.cpp
    Sources/HeaderLayout.cpp

    # Core
    Sources/HomePage.cpp
    Sources/Auth.cpp
    Sources/Register.cpp
    Sources/ForgotPassowrd.cpp
    Sources/Login.cpp
    Sources/IWindowFactory.cpp
    Sources/IForgotPassword.cpp
    Sources/ILoginWindow.cpp
    Sources/IRegisterWindow.cpp

    # Headers
    Headers/HomePage.h
    Headers/Auth.h
    Headers/Register.h
    Headers/ForgotPassowrd.h
    Headers/Login.h
    Headers/Layouts.h
    Headers/HeaderLayout.h

    Headers/layouts/LeftLayout.h
    Headers/layouts/RightLayout.h
    Headers/layouts/CenterLayout.h



    # Interfaces
    Headers/interfaces/IWindowFactory.h
    Headers/interfaces/ILoginWindow.h
    Headers/interfaces/IRegisterWindow.h
    Headers/interfaces/IForgotPassword.h
)

# Создание исполняемого файла
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Weather
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        Headers/Window.h Headers/Window.cpp
        Headers/GoogleAuthServer.h Headers/GoogleAuthServer.cpp
    )
else()
    add_executable(Weather
        ${PROJECT_SOURCES}
    )
endif()

# Линковка с Qt
target_link_libraries(Weather PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

# Настройки для macOS/Windows
set_target_properties(Weather PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# Установка
include(GNUInstallDirs)
install(TARGETS Weather
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Qt6 финализация
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Weather)
endif()
