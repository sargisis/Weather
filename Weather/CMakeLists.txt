cmake_minimum_required(VERSION 3.10)

project(Weather VERSION 0.1 LANGUAGES CXX)

# Set CMake policies
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt libraries
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)



# Set project sources
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    HomePage.cpp
    HomePage.h
    LeftLayout.cpp
    LeftLayout.h
    RightLayout.cpp
    RightLayout.h
    CenterLayout.cpp
    CenterLayout.h
    Layouts.cpp
    Layouts.h
)

# Configure target based on Qt version
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Weather
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        HeaderLayout.h HeaderLayout.cpp
        Auth.h Auth.cpp
        Register.h Register.cpp
        ForgotPassowrd.h ForgotPassowrd.cpp
    )
else()
    add_executable(Weather
        ${PROJECT_SOURCES}
    )
endif()

# Link Qt libraries
target_link_libraries(Weather PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

# Set target properties
set_target_properties(Weather PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# Installation settings
include(GNUInstallDirs)
install(TARGETS Weather
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize executable for Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Weather)
endif()
