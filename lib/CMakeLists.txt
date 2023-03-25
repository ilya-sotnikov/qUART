include(FetchContent)

if(NOT EXISTS ${QCUSTOMPLOT_SRC_DIR})
FetchContent_Declare(
    QCustomPlot
    URL https://www.qcustomplot.com/release/2.1.1/QCustomPlot-source.tar.gz
)
endif()

add_subdirectory(qcustomplot)
