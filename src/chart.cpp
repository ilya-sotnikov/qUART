#include "chart.h"

/**
 * @brief Construct a new Chart object
 *
 * @param parent
 */
Chart::Chart(QWidget *parent) : QWidget{ parent }
{
    constexpr qreal lineWidth{ 1 };
    constexpr qreal plotScatterSize{ 20 };
    constexpr QColor plotSelectionColor{ QColor{ 80, 80, 255 } };

    auto layout{ new QHBoxLayout{ this } };
    layout->addWidget(customPlot);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    // customPlot->setSelectionRectMode(QCP::srmZoom);
    customPlot->setAntialiasedElements(QCP::aePlottables);

    plot->setSelectable(QCP::stSingleData);
    plot->setPen(QPen{ QBrush{ Qt::black }, lineWidth });
    plot->setScatterStyle(
            QCPScatterStyle{ QCPScatterStyle::ssCross, Qt::transparent, plotScatterSize });
    plot->selectionDecorator()->setScatterStyle(QCPScatterStyle{
            QCPScatterStyle::ssCross, QPen{ QBrush{ plotSelectionColor }, lineWidth },
            QBrush{ plotSelectionColor }, plotScatterSize });

    spectrum->setSelectable(QCP::stSingleData);
    spectrum->setPen(QPen{ QBrush{ Qt::black }, lineWidth });
    spectrum->setBrush(QBrush{ Qt::gray });
    spectrum->setWidth(1);

    customPlot->replot();

    timer->setInterval(updateInterval);
    timer->start();

    connect(plot, qOverload<const QCPDataSelection &>(&QCPGraph::selectionChanged), this,
            &Chart::updateSelectedPoint);
    connect(spectrum, qOverload<const QCPDataSelection &>(&QCPBars::selectionChanged), this,
            &Chart::updateSelectedPoint);

    connect(customPlot, &QCustomPlot::mouseRelease, this, [this]() { autoScaleAxes = false; });
    connect(customPlot, &QCustomPlot::mouseWheel, this, [this]() { autoScaleAxes = false; });

    connect(timer, &QTimer::timeout, this, [this]() {
        if (needsUpdate) {
            needsUpdate = false;
            updateChart();
        }
    });
}

/**
 * @brief Updates the data on the chart, rescales axes and replots
 *
 */
void Chart::updateChart()
{
    auto plotData{ chartDataContainer.getPlot(showLastPoints) };

    // Needed because we recreate the last N points from the plotData internally
    ChartDataContainer::SpectrumData spectrumData;
    if (appendToSpectrum)
        spectrumData = chartDataContainer.getSpectrum(showLastPoints);

    if (chartType == ChartType::plot) {
        plot->setData(plotData.keys, plotData.values, true);
        plot->setVisible(true);
        spectrum->setVisible(false);
    } else if (chartType == ChartType::spectrum) {
        spectrum->setData(spectrumData.keys, spectrumData.values, true);
        plot->setVisible(false);
        spectrum->setVisible(true);
    }

    if (autoScaleAxes)
        customPlot->rescaleAxes(true);

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
    autoScaleAxes = true;

    if (chartType == ChartType::spectrum)
        chartType = ChartType::plot;
    else if (chartType == ChartType::plot)
        chartType = ChartType::spectrum;

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
 * @brief Sets raw data to the chart, useful for loading data from a file
 *
 * Note that you should choose the corresponding chart type (plot or spectrum).
 *
 * @param rawData
 */
void Chart::setRawData(QList<qreal> &rawData)
{
    chartDataContainer.clear();
    if (chartType == ChartType::plot) {
        addData(rawData); // restore the plot and the spectrum
    } else if (chartType == ChartType::spectrum) {
        chartDataContainer.setRawSpectrumData(rawData); // impossible to restore the plot
        rawData.clear();
    }
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
void Chart::addData(QList<qreal> &receivedData)
{
    chartDataContainer.append(receivedData, appendToPlot, appendToSpectrum);
    needsUpdate = true;
    receivedData.clear();
}

/**
 * @brief Rescales axes and replots
 *
 */
void Chart::resetZoom()
{
    autoScaleAxes = true;
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

    if (chartType == ChartType::plot) {
        auto plotData{ chartDataContainer.getPlot(showLastPoints) };
        selectedPoint.setX(plotData.keys.at(chartX));
        selectedPoint.setY(plotData.values.at(chartX));
    } else if (chartType == ChartType::spectrum) {
        auto spectrumData{ chartDataContainer.getSpectrum(showLastPoints) };
        selectedPoint.setX(spectrumData.keys.at(chartX));
        selectedPoint.setY(spectrumData.values.at(chartX));
    }

    emit selectedPointChanged(selectedPoint);
}

/**
 * @brief Returns a data list depending on the current chart type (plot or spectrum)
 *
 * @return const QList<qreal>&
 */
const QList<qreal> &Chart::getData() const
{
    return (chartType == ChartType::plot) ? chartDataContainer.getPlot().values
                                          : chartDataContainer.getSpectrum().values;
}

void Chart::setUpdateInterval(int msec)
{
    timer->stop();
    timer->setInterval(msec);
    timer->start();
}
