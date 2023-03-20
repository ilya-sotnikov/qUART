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

    enum ChartType { plot, spectrum };
    Q_ENUM(ChartType)
    auto getChartType() const { return chartType; };

    bool appendToPlot{ true };
    bool appendToSpectrum{ true };

private:
    QCustomPlot *customPlot{ new QCustomPlot{ this } };
    PlotDataContainer plotDataContainer;
    SpectrumDataContainer spectrumDataContainer;
    Chart::ChartType chartType{ Chart::plot };

public slots:
    void addData(QList<qreal> *receivedData);
    void resetZoom();
    void hideMarker();

private slots:
    void updateSelected(const QPointF &point);

signals:
    void pointSelected(const QPointF point);
};

#endif // CHART_H
