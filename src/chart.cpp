#include "chart.h"

/**
 * @brief Construct a new Chart object
 *
 * @param parent
 */
Chart::Chart(QWidget *parent) : QWidget{ parent }
{
    auto layout{ new QHBoxLayout{ this } };
    layout->addWidget(customPlot);

    customPlot->addGraph();
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    // customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    // customPlot->setSelectionRectMode(QCP::srmZoom);
    customPlot->graph()->setSelectable(QCP::stSingleData);
    customPlot->setSelectionRectMode(QCP::srmZoom);
    customPlot->replot();

    connect(customPlot->graph(), qOverload<const QCPDataSelection &>(&QCPGraph::selectionChanged),
            this, &Chart::updateSelectedPoint);

    // plotDataContainer.append(QList<qreal>{0});
    // plotDataContainer.append(QList<qreal>{1, 2, 3, 4, 5});
    // plotDataContainer.append(QList<qreal>{10, 2});
    // plotDataContainer.append(QList<qreal>{-2, -5});
    // spectrumDataContainer.append(QList<qreal>{0});
    // spectrumDataContainer.append(QList<qreal>{1, 2, 3, 4, 5});
    // spectrumDataContainer.append(QList<qreal>{10, 2});
    // spectrumDataContainer.append(QList<qreal>{-2, -5});
}
void Chart::test(const QCPDataSelection &selection)
{
    qDebug() << selection;
}

// /**
//  * @brief Destroy the Chart object and delete its data
//  *
//  */
// Chart::~Chart() {
//   delete dataPlot;
//   delete dataSpectrum;
// }

/**
 * @brief Update data on the chart
 *
 */
void Chart::updateChart()
{
    if (chartType == Chart::plot) {
        customPlot->graph()->setData(plotDataContainer.getKeys(), plotDataContainer.getValues(),
                                     true);
    } else if (chartType == Chart::spectrum) {
        customPlot->graph()->setData(spectrumDataContainer.getKeys(),
                                     spectrumDataContainer.getValues(), true);
    }
    customPlot->rescaleAxes();
    customPlot->replot();
}

/**
 * @brief Change the chart type
 *
 * If the current type is spectrum, it changes type to plot and vice versa.
 *
 */
void Chart::changeType()
{
    if (chartType == Chart::spectrum) {
        customPlot->graph()->setLineStyle(QCPGraph::lsLine);
        chartType = Chart::plot;
    } else if (chartType == Chart::plot) {
        customPlot->graph()->setLineStyle(QCPGraph::lsImpulse);
        chartType = Chart::spectrum;
    }
    updateChart();
}

/**
 * @brief Delete all data and update the chart
 *
 */
void Chart::clear()
{
    plotDataContainer.clear();
    spectrumDataContainer.clear();
    updateChart();
}

/**
 * @brief Add raw data to the chart
 *
 * Note that you should choose the corresponding chart type (plot or spectrum).
 *
 * @param rawData
 */
void Chart::addRawData(QList<qreal> *rawData)
{
    // dataPlot->clear();
    // dataSpectrum->clear();
    // if (chartType == Chart::plot) {
    //   addData(rawData);
    // } else if (chartType == Chart::spectrum) {
    //   dataSpectrum->append(*rawData);
    //   updateChart();
    //   rawData->clear();
    // }
}

/**
 * @brief Add data to the chart
 *
 * If the current chart type is plot, then plots data.
 * If it's spectrum, then plot frequency of an each data point.
 *
 * @param receivedData
 */
void Chart::addData(QList<qreal> *receivedData)
{
    if (appendToPlot)
        plotDataContainer.append(*receivedData);

    if (appendToSpectrum)
        spectrumDataContainer.append(*receivedData);

    updateChart();
    receivedData->clear();
}

/**
 * @brief Enable autoscaling and replot
 *
 */
void Chart::resetZoom()
{
    customPlot->rescaleAxes();
    customPlot->replot();
}

/**
 * @brief Return the selected data point from its coordinates and show marker
 * at this position
 *
 * @param point
 */
void Chart::updateSelectedPoint(const QCPDataSelection &selection)
{
    qDebug() << selection;
}

/**
 * @brief Hide the marker
 *
 */
void Chart::hideMarker()
{
    // marker->hide();
    // replot();
}

/**
 * @brief Get a data list depending on the current chart type (plot or
 * spectrum)
 *
 * @return const QList<qreal>&
 */
const QList<qreal> &Chart::getData() const
{
    // return (chartType == Chart::plot) ? *dataPlot : *dataSpectrum;
}
