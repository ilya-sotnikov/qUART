#include "customplot.h"
#include "themeinfo.h"

CustomPlot::CustomPlot(QWidget *parent) : QCustomPlot{ parent }
{
    setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    setSelectionTolerance(20);
    setAntialiasedElements(QCP::aePlottables);

    const auto themeWindowColor{ ThemeInfo::windowColor() };
    const auto themeWindowTextColor{ ThemeInfo::windowTextColor() };

    setBackground(QBrush{ themeWindowColor });
    xAxis->setBasePen(QPen{ themeWindowTextColor });
    xAxis->setTickPen(QPen{ themeWindowTextColor });
    xAxis->setSubTickPen(QPen{ themeWindowTextColor });
    xAxis->setLabelColor(themeWindowTextColor);
    xAxis->setTickLabelColor(themeWindowTextColor);

    yAxis->setBasePen(QPen{ themeWindowTextColor });
    yAxis->setTickPen(QPen{ themeWindowTextColor });
    yAxis->setSubTickPen(QPen{ themeWindowTextColor });
    yAxis->setLabelColor(themeWindowTextColor);
    yAxis->setTickLabelColor(themeWindowTextColor);
}
