#include "chart.h"

Chart::Chart(QWidget *parent) : QWidget{ parent }
{
    constexpr qreal lineWidth{ 1 };
    constexpr qreal plotScatterSize{ 10 };
    constexpr auto plotSelectionColor{ QColor{ 80, 80, 255 } };

    const auto layout{ new QHBoxLayout{ this } };
    layout->addWidget(customPlot);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->setSelectionTolerance(20);
    customPlot->setAntialiasedElements(QCP::aePlottables);

    graph->setSelectable(QCP::stSingleData);
    graph->setPen(QPen{ QBrush{ Qt::black }, lineWidth });
    graph->setScatterStyle(
            QCPScatterStyle{ QCPScatterStyle::ssCross, Qt::transparent, plotScatterSize });
    graph->selectionDecorator()->setScatterStyle(QCPScatterStyle{
            QCPScatterStyle::ssCross, QPen{ QBrush{ plotSelectionColor }, lineWidth + 1 },
            QBrush{ plotSelectionColor }, plotScatterSize });

    customPlot->replot();

    timer->setInterval(updateInterval);
    timer->start();

    connect(graph, qOverload<const QCPDataSelection &>(&QCPGraph::selectionChanged), this,
            &Chart::updateSelectedPoint);

    connect(timer, &QTimer::timeout, this, [this]() {
        if (needsUpdate) {
            needsUpdate = false;
            updateChart();
        }
    });

    connect(customPlot, &QCustomPlot::mousePress, this, [this](const QMouseEvent *event) {
        if (event->button() == Qt::RightButton)
            customPlot->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);
        else
            customPlot->setSelectionRectMode(QCP::SelectionRectMode::srmNone);
    });
}

/**
 * @brief Updates the data on the chart, rescales axes if needed, updates the selection and replots
 *
 */
void Chart::updateChart() const
{
    if (chartType == ChartType::plot)
        graph->setData(chartDataContainer.getPlotLast());
    else if (chartType == ChartType::spectrum)
        graph->setData(chartDataContainer.getSpectrumLast());

    if (autoscaleX)
        graph->keyAxis()->rescale(true);

    if (autoscaleY)
        graph->valueAxis()->rescale(true);

    const auto selection{ graph->selection() };
    if (!selection.isEmpty())
        updateSelectedPoint(selection);

    customPlot->replot();
}

/**
 * @brief Changes the chart type and updates the chart
 *
 * If the current type is spectrum, it changes type to plot and vice versa.
 *
 */
void Chart::changeType()
{
    setAutoscale(true, true);

    if (chartType == ChartType::spectrum) {
        chartType = ChartType::plot;
        graph->setLineStyle(QCPGraph::lsLine);
        graph->keyAxis()->setTickLengthIn(defaultTickLength);
        graph->keyAxis()->setSubTickLengthIn(defaultSubTickLength);
    } else if (chartType == ChartType::plot) {
        chartType = ChartType::spectrum;
        graph->setLineStyle(QCPGraph::lsImpulse);
        graph->keyAxis()->setTickLengthIn(0);
        graph->keyAxis()->setSubTickLengthIn(0);
    }

    customPlot->deselectAll();

    needsUpdate = true;
}

/**
 * @brief Deletes all data and updates the chart
 *
 */
void Chart::clear()
{
    chartDataContainer.clear();
    needsUpdate = true;
}

/**
 * @brief Adds data to the chart
 *
 * @param receivedData
 *
 */
void Chart::addData(const QSharedPointer<const QList<qreal>> receivedData)
{
    chartDataContainer.append(*receivedData, appendToPlot, appendToSpectrum);
    needsUpdate = true;
}

/**
 * @brief Sets spectrumData to raw data, useful when loading data from a file
 *
 * @param rawData
 *
 */
void Chart::setRawSpectrumData(const QCPGraphDataContainer &rawData)
{
    chartDataContainer.setRawSpectrumData(rawData);
    needsUpdate = true;
}

/**
 * @brief Sets autoscaling for both axes, rescales them and replots
 *
 */
void Chart::resetZoom()
{
    setAutoscale(true, true);
    customPlot->rescaleAxes(true);
    customPlot->replot();
}

/**
 * @brief Updates the selected data point from its coordinates and emits a signal
 *
 * @param selection
 *
 */
void Chart::updateSelectedPoint(const QCPDataSelection &selection) const
{
    if (selection.isEmpty())
        return;

    const auto chartX{ selection.dataRange().begin() };
    QPointF selectedPoint{};
    selectedPoint.setX(chartX);

    QCPGraphDataContainer::const_iterator pointIter;
    if (chartType == ChartType::plot)
        pointIter = chartDataContainer.getPlotLast()->at(chartX);
    else if (chartType == ChartType::spectrum)
        pointIter = chartDataContainer.getSpectrumLast()->at(chartX);

    selectedPoint.setX(pointIter->mainKey());
    selectedPoint.setY(pointIter->mainValue());

    emit selectedPointChanged(selectedPoint);
}

/**
 * @brief Sets update interval of a chart
 *
 * @param msec Update interval in milliseconds
 */
void Chart::setUpdateInterval(const int msec) const
{
    customPlot->deselectAll();

    timer->stop();
    timer->setInterval(msec);
    timer->start();
}

/**
 * @brief Sets value axis scaling (log or linear)
 *
 * @param log If true then log, otherwise linear
 */
void Chart::setLogarithmic(const bool log)
{
    if (log) {
        customPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
        customPlot->yAxis->setTicker(QSharedPointer<QCPAxisTickerLog>::create());
    } else {
        customPlot->yAxis->setScaleType(QCPAxis::stLinear);
        customPlot->yAxis->setTicker(QSharedPointer<QCPAxisTicker>::create());
    }

    needsUpdate = true;
};

/**
 * @brief Enables or disables autoscaling for both axes
 *
 * @param autoscaleX
 * @param autoscaleY
 *
 */
void Chart::setAutoscale(const bool autoscaleX, const bool autoscaleY)
{
    setAutoscaleX(autoscaleX);
    setAutoscaleY(autoscaleY);
};

/**
 * @brief Enables or disables autoscaling for the X axis (keys)
 *
 * @param autoscale
 *
 */
void Chart::setAutoscaleX(const bool autoscale)
{
    const auto prevAutoScaleX{ autoscaleX };
    autoscaleX = autoscale;

    needsUpdate = true;

    if ((prevAutoScaleX != autoscaleX))
        emit autoscaleChanged(autoscaleX, autoscaleY);
}

/**
 * @brief Enables or disables autoscaling for the Y axis (values)
 *
 * @param autoscale
 *
 */
void Chart::setAutoscaleY(const bool autoscale)
{
    const auto prevAutoScaleY{ autoscaleY };
    autoscaleY = autoscale;

    needsUpdate = true;

    if ((prevAutoScaleY != autoscaleY))
        emit autoscaleChanged(autoscaleX, autoscaleY);
}
