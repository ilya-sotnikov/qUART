#ifndef CHART_H
#define CHART_H

#include "chartdatacontainers.h"

#include "qcustomplot.h"
#include <qwidget.h>

class Chart : public QWidget
{
    Q_OBJECT

public:
    explicit Chart(QWidget *parent = nullptr);
    void updateChart();
    void changeType();
    void clear();
    void setRawData(QList<qreal> &rawData);
    const QList<qreal> &getData() const;

    enum class ChartType { plot, spectrum };
    Q_ENUM(ChartType)
    auto getChartType() const { return chartType; };

    bool appendToPlot{ true };
    bool appendToSpectrum{ true };

private:
    QCustomPlot *customPlot{ new QCustomPlot{ this } };
    QCPGraph *plot{ customPlot->addGraph() };
    QCPBars *spectrum{ new QCPBars{ customPlot->xAxis, customPlot->yAxis } };
    Chart::ChartType chartType{ ChartType::plot };
    PlotDataContainer plotDataContainer;
    SpectrumDataContainer spectrumDataContainer;

public slots:
    void addData(QList<qreal> &receivedData);
    void resetZoom();

private slots:
    void updateSelectedPoint(const QCPDataSelection &selection);

signals:
    void selectedPointChanged(const QPointF selectedPoint);
};

#endif // CHART_H
