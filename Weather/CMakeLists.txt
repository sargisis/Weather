# Минимальная требуемая версия CMake. 3.10 вполне достаточно.
cmake_minimum_required(VERSION 3.10)

# Объявление проекта, его имени, версии и используемых языков.
project(Weather VERSION 0.1 LANGUAGES CXX)

# Включение автоматической обработки файлов UI (User Interface),
# MOC (Meta-Object Compiler) и RCC (Resource Compiler).
# Это стандартная практика для Qt проектов.
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Установка стандарта C++ до C++23 и требование его использования.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск пакета Qt.
# Мы явно указываем Qt6 и требуем компоненты Widgets, Network и Svg.
# Если Qt6 не будет найден, CMake будет искать Qt5.
# REQUIRED означает, что если компоненты не будут найдены, CMake выдаст ошибку.
find_package(Qt6 CONFIG REQUIRED COMPONENTS Widgets Network Svg)
# Если вы хотите более явное переключение между Qt5 и Qt6, можете использовать:
# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Svg)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Svg)

# Добавление директорий с заголовочными файлами.
# Используйте CMAKE_CURRENT_SOURCE_DIR для ссылки на корневую директорию проекта.
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers/layouts
    ${CMAKE_CURRENT_SOURCE_DIR}/Headers/interfaces
)

# Определение всех исходных файлов проекта (cpp и h для AUTOMOC).
# Qt Creator обычно сам добавляет эти файлы, но ручное определение полезно для ясности.
set(PROJECT_SOURCES
    main.cpp

    # Layouts
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

    # Headers (для AUTOMOC и autocompletion)
    Headers/HomePage.h
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

    # Interfaces
    Headers/interfaces/IWindowFactory.h
    Headers/interfaces/ILoginWindow.h
    Headers/interfaces/IRegisterWindow.h
    Headers/interfaces/IForgotPassword.h

    # Ресурсные файлы (для AUTORCC)
    resources/icons.qrc
)

# Создание исполняемого файла.
# Используем qt_add_executable для Qt6 для правильной обработки.
# MANUAL_FINALIZATION позволяет вручную вызвать qt_finalize_executable позже.
if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_executable(Weather
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        Sources/Window.cpp # Добавление cpp файлов, не указанных выше
        Sources/GoogleAuthServer.cpp # Добавление cpp файлов, не указанных выше
    )
else() # Ветка для Qt5, если потребуется (лучше явно указать все файлы)
    add_executable(Weather
        ${PROJECT_SOURCES}
        Sources/GoogleAuthServer.cpp
    )
endif()

# Линковка исполняемого файла со всеми необходимыми библиотеками Qt.
# PRIVATE означает, что эти зависимости нужны только для сборки этого таргета.
target_link_libraries(Weather PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Svg # Модуль SVG теперь включен
)

# Настройка свойств исполняемого файла для macOS (BUNDLE) и Windows (WIN32_EXECUTABLE).
set_target_properties(Weather PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# Установка исполняемого файла и библиотек в соответствии с GNUInstallDirs.
include(GNUInstallDirs)
install(TARGETS Weather
    BUNDLE DESTINATION . # Для macOS bundles
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Финализация исполняемого файла Qt6. Это обязательный шаг для Qt6 после qt_add_executable(MANUAL_FINALIZATION).
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Weather)
endif()
