cmake_minimum_required(VERSION 3.24)

project(qUART VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(QT_VERSION_MAJOR 6)

set(CMAKE_AUTORCC ON)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

# QCustomPlot
set(QCUSTOMPLOT_SRC_DIR "")

add_subdirectory(lib)
add_subdirectory(src)
