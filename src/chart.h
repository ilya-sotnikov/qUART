#ifndef CHART_H
#define CHART_H

#include "chartdatacontainer.h"

#include "qcustomplot.h"
#include <qwidget.h>

/**
 * @brief Displays a chart (in plot mode or spectrum mode)
 */
class Chart : public QWidget
{
    Q_OBJECT

public:
    explicit Chart(QWidget *parent = nullptr);

    enum class ChartType { plot, spectrum };
    Q_ENUM(ChartType)
    auto getChartType() const { return chartType; };
    const QList<qreal> &getData() const { return chartDataContainer.getPlot().values; }
    void updateChart();
    void changeType();
    void clear();
    void setRawData(QList<qreal> &rawData);
    void setUpdateInterval(int msec);
    auto getUpdateInterval() const { return updateInterval; };
    void requestUpdate() { needsUpdate = true; }
    void setPlotLogarithmic(bool log)
    {
        if (log)
            plot->valueAxis()->setScaleType(QCPAxis::stLogarithmic);
        else
            plot->valueAxis()->setScaleType(QCPAxis::stLinear);

        needsUpdate = true;
    };
    void setSpectrumLogarithmic(bool log)
    {
        if (log)
            spectrum->valueAxis()->setScaleType(QCPAxis::stLogarithmic);
        else
            spectrum->valueAxis()->setScaleType(QCPAxis::stLinear);

        needsUpdate = true;
    };

    bool appendToPlot{ true };
    bool appendToSpectrum{ true };

private:
    QCustomPlot *customPlot{ new QCustomPlot{ this } };
    QCPGraph *plot{ customPlot->addGraph() };
    QCPBars *spectrum{ new QCPBars{ customPlot->xAxis, customPlot->yAxis } };
    Chart::ChartType chartType{ ChartType::plot };
    ChartDataContainer chartDataContainer;
    QTimer *timer{ new QTimer{ this } };
    qsizetype showLastPoints{ -1 };
    bool autoScaleAxes{ true };
    int updateInterval{ 33 };
    bool needsUpdate{ false };

public slots:
    void resetZoom();
    void addData(QList<qreal> &receivedData);
    void setShowLastPoints(qsizetype n) { showLastPoints = n; }

private slots:
    void updateSelectedPoint(const QCPDataSelection &selection);

signals:
    void selectedPointChanged(const QPointF selectedPoint);
};

#endif // CHART_H
