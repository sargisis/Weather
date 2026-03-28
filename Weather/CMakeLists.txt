cmake_minimum_required(VERSION 3.10)

project(Weather VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Gui 
    Widgets 
    Network 
    Svg
    Positioning 
    TextToSpeech
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers/layouts
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers/Interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers/NavigationEngine
)

set(PROJECT_SOURCES
    main.cpp

    # Layouts
    Sources/Layouts/RightLayout.cpp
    Sources/Layouts/CenterLayout.cpp
    Sources/Layouts/NavigationLayout.cpp
    Sources/Layouts/ForecastChartWidget.cpp
    Sources/Layouts.cpp
    Sources/HeaderLayout.cpp

    # Core
    Sources/HomePage.cpp
    Sources/MapWidget.cpp
    Sources/Auth.cpp
    Sources/Register.cpp
    Sources/ForgotPassowrd.cpp
    Sources/Login.cpp
    Sources/IWindowFactory.cpp
    Sources/IForgotPassword.cpp
    Sources/ILoginWindow.cpp
    Sources/IRegisterWindow.cpp
    Sources/GoogleAuthServer.cpp
    Sources/INavIterface.cpp


    # Headers
    Headers/HomePage.h
    Headers/MapWidget.h
    Headers/Auth.h
    Headers/Register.h
    Headers/ForgotPassowrd.h
    Headers/Login.h
    Headers/Layouts.h
    Headers/HeaderLayout.h
    Headers/Window.h
    Headers/GoogleAuthServer.h

    Headers/layouts/RightLayout.h
    Headers/layouts/CenterLayout.h
    Headers/layouts/NavigationLayout.h
    Headers/layouts/ForecastChartWidget.h

    Headers/Interfaces/IWindowFactory.h
    Headers/Interfaces/ILoginWindow.h
    Headers/Interfaces/IRegisterWindow.h
    Headers/Interfaces/IForgotPassword.h
    Headers/Interfaces/INavIterface.h

    # NavigationEngine
    Sources/NavigationEngine/gpsservice.cpp
    Sources/NavigationEngine/routingservice.cpp
    Sources/NavigationEngine/routetracker.cpp
    Sources/NavigationEngine/voiceservice.cpp
    Sources/NavigationEngine/navigationengine.cpp

    Headers/NavigationEngine/route.h
    Headers/NavigationEngine/gpsservice.h
    Headers/NavigationEngine/routingservice.h
    Headers/NavigationEngine/routetracker.h
    Headers/NavigationEngine/voiceservice.h
    Headers/NavigationEngine/navigationengine.h

    # Resources
    resources/icons.qrc
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_executable(Weather
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(Weather
        ${PROJECT_SOURCES}

    )
endif()

target_link_libraries(Weather PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Svg
    Qt${QT_VERSION_MAJOR}::Positioning
    Qt${QT_VERSION_MAJOR}::TextToSpeech
)

# Настройка для macOS и Windows
set_target_properties(Weather PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# 🔽 Подключение Info.plist (для macOS)
if(APPLE)
    set_target_properties(Weather PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Special/Info.plist"
    )
endif()

include(GNUInstallDirs)
install(TARGETS Weather
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Weather)
endif()
