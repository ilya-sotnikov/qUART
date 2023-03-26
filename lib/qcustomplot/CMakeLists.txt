if (NOT EXISTS ${QCUSTOMPLOT_SRC_DIR})
    FetchContent_MakeAvailable(QCustomPlot)
    set(QCUSTOMPLOT_SRC_DIR ${qcustomplot_SOURCE_DIR})
endif()

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_library(QCustomPlot STATIC)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui PrintSupport)
target_include_directories(QCustomPlot PUBLIC ${QCUSTOMPLOT_SRC_DIR})
target_sources(QCustomPlot PRIVATE
    ${QCUSTOMPLOT_SRC_DIR}/qcustomplot.cpp
    ${QCUSTOMPLOT_SRC_DIR}/qcustomplot.h
)
target_link_libraries(QCustomPlot PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

if(MSVC)
    target_compile_options(QCustomPlot PRIVATE /W4)
else()
    target_compile_options(QCustomPlot PRIVATE -Wall -Wextra)
    set(SANITIZERS "-fsanitize=address,undefined,leak")
    target_compile_options(QCustomPlot PRIVATE "$<$<CONFIG:DEBUG>:${SANITIZERS}>")
    target_link_options(QCustomPlot PRIVATE "$<$<CONFIG:DEBUG>:${SANITIZERS}>")
endif()
