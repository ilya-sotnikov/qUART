#ifndef CHART_H
#define CHART_H

#include "chartdatacontainer.h"

#include "qcustomplot.h"

#include <QWidget>

class Chart : public QWidget
{
    Q_OBJECT

public:
    explicit Chart(QWidget *parent = nullptr);
    // ~Chart();
    void updateChart();
    void changeType();
    void clear();
    void addRawData(QList<qreal> *rawData);
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
    PlotDataContainer plotDataContainer;
    SpectrumDataContainer spectrumDataContainer;
    Chart::ChartType chartType{ ChartType::plot };

public slots:
    void addData(QList<qreal> *receivedData);
    void resetZoom();

private slots:
    void updateSelectedPoint(const QCPDataSelection &selection);

signals:
    void selectedPointChanged(const QPointF selectedPoint);
};

#endif // CHART_H
