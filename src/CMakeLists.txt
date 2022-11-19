cmake_minimum_required(VERSION 3.5)

project(qUART VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(QWT_LIB_PATH "")
set(QWT_INCLUDE "")

if(NOT EXISTS ${QWT_INCLUDE})
    message(FATAL_ERROR "Set QWT_INCLUDE to location of qwt/include")
endif()

if(NOT EXISTS ${QWT_LIB_PATH})
    message(FATAL_ERROR "Set QWT_LIB_PATH to location of qwt/lib")
endif()

find_library(QWT_LIB qwt HINTS ${QWT_LIB_PATH})

#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
set(QT_VERSION_MAJOR 6)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets SerialPort)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.h
        MainWindow.cpp
        Plot.h
        Plot.cpp
        SerialTransceiver.h
        SerialTransceiver.cpp
        PortSettingsDialog.h
        PortSettingsDialog.cpp
        DataSettingsDialog.h
        DataSettingsDialog.cpp
        PortsInfoDialog.h
        PortsInfoDialog.cpp
        TextWidget.h
        TextWidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../icons/icons.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qUART
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET qUART APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(qUART SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(qUART
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(qUART PRIVATE "${QWT_INCLUDE}")
target_link_libraries(qUART PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::SerialPort ${QWT_LIB})

set_target_properties(qUART PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)


if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qUART)
endif()
