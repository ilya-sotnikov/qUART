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
    auto getPlotData() const { return chartDataContainer.getPlot(); }
    auto getSpectrumData() const { return chartDataContainer.getSpectrum(); }
    void updateChart();
    void changeType();
    void clear();
    void setRawSpectrumData(const QCPGraphDataContainer &rawData);
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
    void setAutoscale(bool x, bool y)
    {
        setAutoscaleX(x);
        setAutoscaleY(y);
    };
    void setAutoscaleX(bool x)
    {
        auto prevAutoScaleX{ autoscaleX };
        autoscaleX = x;

        needsUpdate = true;

        if ((prevAutoScaleX != autoscaleX))
            emit autoscaleChanged(autoscaleX, autoscaleY);
    }
    void setAutoscaleY(bool y)
    {
        auto prevAutoScaleY{ autoscaleY };
        autoscaleY = y;

        needsUpdate = true;

        if ((prevAutoScaleY != autoscaleY))
            emit autoscaleChanged(autoscaleX, autoscaleY);
    }
    bool getAutoScaleX() { return autoscaleX; };
    bool getAutoScaleY() { return autoscaleY; };

    bool appendToPlot{ true };
    bool appendToSpectrum{ true };

private:
    QCustomPlot *customPlot{ new QCustomPlot{ this } };
    CustomGraph *graph{ new CustomGraph{ customPlot->xAxis, customPlot->yAxis } };
    Chart::ChartType chartType{ ChartType::plot };
    ChartDataContainer chartDataContainer;
    QTimer *timer{ new QTimer{ this } };
    bool autoscaleX{ true };
    bool autoscaleY{ true };
    int updateInterval{ 33 };
    bool needsUpdate{ false };
    int defaultTickLength{ graph->keyAxis()->tickLengthIn() };
    int defaultSubTickLength{ graph->keyAxis()->subTickLengthIn() };

public slots:
    void resetZoom();
    void addData(const QSharedPointer<const QList<qreal>> receivedData);
    void setShowLastPoints(qsizetype n) { chartDataContainer.setLastPointsCount(n); }

private slots:
    void updateSelectedPoint(const QCPDataSelection &selection);

signals:
    void selectedPointChanged(const QPointF selectedPoint);
    void autoscaleChanged(bool x, bool y);
};

#endif // CHART_H
