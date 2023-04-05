#include "chart.h"

/**
 * @brief Construct a new Chart object
 *
 * @param parent
 */
Chart::Chart(QWidget *parent) : QWidget{ parent }
{
    constexpr qreal lineWidth{ 1 };
    constexpr qreal plotScatterSize{ 10 };
    constexpr QColor plotSelectionColor{ QColor{ 80, 80, 255 } };

    auto layout{ new QHBoxLayout{ this } };
    layout->addWidget(customPlot);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    // customPlot->setSelectionRectMode(QCP::srmZoom);
    customPlot->setSelectionTolerance(20);
    customPlot->setAntialiasedElements(QCP::aePlottables);

    graph->setSelectable(QCP::stSingleData);
    graph->setPen(QPen{ QBrush{ Qt::black }, lineWidth });
    graph->setScatterStyle(
            QCPScatterStyle{ QCPScatterStyle::ssCross, Qt::transparent, plotScatterSize });
    graph->selectionDecorator()->setScatterStyle(QCPScatterStyle{
            QCPScatterStyle::ssCross, QPen{ QBrush{ plotSelectionColor }, lineWidth + 1 },
            QBrush{ plotSelectionColor }, plotScatterSize });

    // spectrum->setSelectable(QCP::stSingleData);
    // spectrum->setPen(QPen{ QBrush{ Qt::black }, lineWidth });
    // spectrum->setBrush(QBrush{ Qt::gray });
    // spectrum->setWidth(0.5);

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

    connect(customPlot, &QCustomPlot::mousePress, this, [this](QMouseEvent *event) {
        if (event->button() == Qt::RightButton)
            customPlot->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);
        else
            customPlot->setSelectionRectMode(QCP::SelectionRectMode::srmNone);
    });
}

/**
 * @brief Updates the data on the chart, rescales axes and replots
 *
 */
void Chart::updateChart()
{
    if (chartType == ChartType::plot)
        graph->setData(chartDataContainer.getPlotLast());
    else if (chartType == ChartType::spectrum)
        graph->setData(chartDataContainer.getSpectrumLast());

    if (autoscaleX)
        graph->keyAxis()->rescale(true);

    if (autoscaleY)
        graph->valueAxis()->rescale(true);

    auto selection{ graph->selection() };
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
 * If the current chart type is plot, then plots data.
 * If it's spectrum, then plot frequency of an each data point.
 *
 * @param receivedData
 */
void Chart::addData(const QSharedPointer<const QList<qreal>> receivedData)
{
    chartDataContainer.append(*receivedData, appendToPlot, appendToSpectrum);
    needsUpdate = true;
}

void Chart::setRawSpectrumData(const QCPGraphDataContainer &rawData)
{
    chartDataContainer.setRawSpectrumData(rawData);
    needsUpdate = true;
}

/**
 * @brief Rescales axes and replots
 *
 */
void Chart::resetZoom()
{
    setAutoscale(true, true);
    customPlot->rescaleAxes(true);
    customPlot->replot();
}

/**
 * @brief Returns the selected data point from its coordinates
 *
 * @param selection
 */
void Chart::updateSelectedPoint(const QCPDataSelection &selection)
{
    if (selection.isEmpty())
        return;

    auto chartX{ selection.dataRange().begin() };
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

void Chart::setUpdateInterval(int msec)
{
    customPlot->deselectAll();

    timer->stop();
    timer->setInterval(msec);
    timer->start();
}
