cmake_minimum_required(VERSION 3.5)

project(qUART VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
set(QT_VERSION_MAJOR 6)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets SerialPort)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.h
        mainwindow.cpp
        chart.h
        chart.cpp
        chartdatacontainer.h
        chartdatacontainer.cpp
        serialtransceiver.h
        serialtransceiver.cpp
        portsettingsdialog.h
        portsettingsdialog.cpp
        datasettingsdialog.h
        datasettingsdialog.cpp
        portsinfodialog.h
        portsinfodialog.cpp
        textwidget.h
        textwidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../icons/icons.qrc
)

# QCustomPlot
set(QCUSTOMPLOT_SRC_PATH "")

if(NOT EXISTS ${QCUSTOMPLOT_SRC_PATH})
    message(FATAL_ERROR "Set QCUSTOMPLOT_SRC_PATH to the location of qcustomplot.cpp and qcustomplot.h")
endif()

add_library(QCustomPlot STATIC)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui PrintSupport)
target_include_directories(QCustomPlot PUBLIC ${QCUSTOMPLOT_SRC_PATH})
target_sources(QCustomPlot PRIVATE
    ${QCUSTOMPLOT_SRC_PATH}/qcustomplot.cpp
    ${QCUSTOMPLOT_SRC_PATH}/qcustomplot.h
)
target_link_libraries(QCustomPlot PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::SerialPort  QCustomPlot)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
    set(SANITIZERS "-fsanitize=address,undefined,leak")
    target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:${SANITIZERS}>")
    target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:${SANITIZERS}>")
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ./compile_commands.json ../compile_commands.json)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
