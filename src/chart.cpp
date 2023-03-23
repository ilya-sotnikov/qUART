#include "chart.h"

/**
 * @brief Construct a new Chart object
 *
 * @param parent
 */
Chart::Chart(QWidget *parent) : QWidget{ parent }
{
    constexpr qreal lineWidth{ 2 };
    constexpr qreal plotScatterSize{ 10 };
    constexpr QColor plotSelectionColor{ QColor{ 80, 80, 255 } };

    auto layout{ new QHBoxLayout{ this } };
    layout->addWidget(customPlot);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->setSelectionRectMode(QCP::srmZoom);
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

    connect(plot, qOverload<const QCPDataSelection &>(&QCPGraph::selectionChanged), this,
            &Chart::updateSelectedPoint);
    connect(spectrum, qOverload<const QCPDataSelection &>(&QCPBars::selectionChanged), this,
            &Chart::updateSelectedPoint);
}

/**
 * @brief Updates the data on the chart, rescales axes and replots
 *
 */
void Chart::updateChart()
{
    if (chartType == ChartType::plot) {
        plot->setData(plotDataContainer.getKeys(), plotDataContainer.getValues(), true);
        plot->setVisible(true);
        spectrum->setVisible(false);
    } else if (chartType == ChartType::spectrum) {
        plot->setVisible(false);
        spectrum->setVisible(true);
        spectrum->setData(spectrumDataContainer.getKeys(), spectrumDataContainer.getValues(), true);
    }
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
    if (chartType == ChartType::spectrum)
        chartType = ChartType::plot;
    else if (chartType == ChartType::plot)
        chartType = ChartType::spectrum;

    updateChart();
}

/**
 * @brief Deletes all data and updates the chart
 *
 */
void Chart::clear()
{
    plotDataContainer.clear();
    spectrumDataContainer.clear();
    updateChart();
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
    plotDataContainer.clear();
    spectrumDataContainer.clear();
    if (chartType == ChartType::plot) {
        addData(rawData); // restore the plot and the spectrum
    } else if (chartType == ChartType::spectrum) {
        spectrumDataContainer.setRawData(rawData); // impossible to restore the plot
        updateChart();
        rawData.clear();
    }
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
    if (appendToPlot)
        plotDataContainer.append(receivedData);

    if (appendToSpectrum)
        spectrumDataContainer.append(receivedData);

    updateChart();
    receivedData.clear();
}

/**
 * @brief Rescales axes and replots
 *
 */
void Chart::resetZoom()
{
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

    auto selectedPointX{ selection.dataRange().begin() };
    QPointF selectedPoint{};
    selectedPoint.setX(selectedPointX);

    if (chartType == ChartType::plot) {
        selectedPoint.setY(plotDataContainer.getValues().at(selectedPointX));
    } else if (chartType == ChartType::spectrum) {
        selectedPoint.setY(spectrumDataContainer.getValues().at(selectedPointX));
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
    return (chartType == ChartType::plot) ? plotDataContainer.getValues()
                                          : spectrumDataContainer.getValues();
}
