#ifndef CHART_H
#define CHART_H

#include "chartdatacontainer.h"
#include "customgraph.h"

#include "qcustomplot.h"
#include <qwidget.h>

/**
 * @brief Displays a chart (in plot mode or spectrum mode)
 *
 */
class Chart : public QWidget
{
    Q_OBJECT

public:
    explicit Chart(QWidget *parent = nullptr);

    enum ChartType { plot, spectrum };
    Q_ENUM(ChartType)
    auto getChartType() const { return chartType; }
    const auto &getPlotData() const { return chartDataContainer.getPlot(); }
    const auto &getSpectrumData() const { return chartDataContainer.getSpectrum(); }
    void updateChart() const;
    void changeType();
    void clear();
    void setRawSpectrumData(const QCPGraphDataContainer &rawData);
    void setUpdateInterval(int msec) const;
    auto getUpdateInterval() const { return updateInterval; }
    void requestUpdate() { needsUpdate = true; }
    void setLogarithmic(const bool log);
    void setAutoscale(const bool autoscaleX, const bool autoscaleY);
    void setAutoscaleX(const bool autoscale);
    void setAutoscaleY(const bool autoscale);
    auto getAutoScaleX() const { return autoscaleX; }
    auto getAutoScaleY() const { return autoscaleY; }

private:
    QCustomPlot *customPlot{ new QCustomPlot{ this } };
    CustomGraph *graph{ new CustomGraph{ customPlot->xAxis, customPlot->yAxis } };
    Chart::ChartType chartType{ ChartType::plot };
    ChartDataContainer chartDataContainer;
    QTimer *timer{ new QTimer{ this } };
    bool appendToPlot{ true };
    bool appendToSpectrum{ true };
    bool autoscaleX{ true };
    bool autoscaleY{ true };
    int updateInterval{ 33 };
    bool needsUpdate{ false };
    const int defaultTickLength{ graph->keyAxis()->tickLengthIn() };
    const int defaultSubTickLength{ graph->keyAxis()->subTickLengthIn() };

public slots:
    void resetZoom();
    void addData(const QSharedPointer<const QList<qreal>> receivedData);
    void setShowLastPoints(const qsizetype n) { chartDataContainer.setLastPointsCount(n); }
    void setAppendToPlot(const bool state) { appendToPlot = state; }
    void setAppendToSpectrum(const bool state) { appendToSpectrum = state; }

private slots:
    void updateSelectedPoint(const QCPDataSelection &selection) const;

signals:
    void selectedPointChanged(const QPointF selectedPoint) const;
    void autoscaleChanged(const bool autoscaleX, const bool autoscaleY) const;
};

#endif // CHART_H
