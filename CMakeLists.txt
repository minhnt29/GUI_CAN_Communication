cmake_minimum_required(VERSION 3.5)

project(CAN_FULL VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS SerialBus Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS SerialBus Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

#target_include_directories(CAN_FULL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/resources)

add_executable(CAN_FULL  ${PROJECT_SOURCES})

target_link_libraries(CAN_FULL PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::SerialBus)



