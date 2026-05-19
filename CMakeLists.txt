cmake_minimum_required(VERSION 3.16)
project(BinaryClock VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)


qt_add_executable(BinaryClock
    src/main.cpp
    src/ClockWidget.cpp
    src/ClockWidget.h
    src/GridStyleWidget.cpp
    src/GridStyleWidget.h
    src/CircleStyleWidget.cpp
    src/CircleStyleWidget.h
    src/SettingsDialog.cpp
    src/SettingsDialog.h
    src/ClockSettings.h
)

qt_add_resources(BinaryClock "appres"
    PREFIX "/"
    FILES resources/app.ico
)

target_link_libraries(BinaryClock PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

set_target_properties(BinaryClock PROPERTIES
    WIN32_EXECUTABLE TRUE
)
