#ifndef CHART_H
#define CHART_H

#include "chartdatacontainer.h"
#include "customgraph.h"

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
    const auto &getPlotData() const { return chartDataContainer.getPlot().values; }
    const auto &getSpectrumData() const { return chartDataContainer.getSpectrumMap(); }
    void updateChart();
    void changeType();
    void clear();
    void setRawSpectrumData(QMap<qreal, qreal> &rawData);
    void setUpdateInterval(int msec);
    auto getUpdateInterval() const { return updateInterval; };
    void requestUpdate() { needsUpdate = true; }
    void setLogarithmic(bool log)
    {
        if (log) {
            customPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
            customPlot->yAxis->setTicker(QSharedPointer<QCPAxisTickerLog>{ new QCPAxisTickerLog });
        } else {
            customPlot->yAxis->setScaleType(QCPAxis::stLinear);
            customPlot->yAxis->setTicker(QSharedPointer<QCPAxisTicker>{ new QCPAxisTicker });
        }

        needsUpdate = true;
    };

    bool appendToPlot{ true };
    bool appendToSpectrum{ true };

private:
    QCustomPlot *customPlot{ new QCustomPlot{ this } };
    CustomGraph *graph{ new CustomGraph{ customPlot->xAxis, customPlot->yAxis } };
    Chart::ChartType chartType{ ChartType::plot };
    ChartDataContainer chartDataContainer;
    QTimer *timer{ new QTimer{ this } };
    qsizetype showLastPoints{ -1 };
    bool autoScaleAxes{ true };
    int updateInterval{ 33 };
    bool needsUpdate{ false };
    int defaultTickLength{ graph->keyAxis()->tickLengthIn() };
    int defaultSubTickLength{ graph->keyAxis()->subTickLengthIn() };

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
