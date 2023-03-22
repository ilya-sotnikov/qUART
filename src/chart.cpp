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

    QList<qreal> testData{ 0, 1, 2, 3, 4, 5, 10, 2, -2, -5 };

    addData(&testData);
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
 * @brief Change the chart type
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
 * @brief Rescale axes and replot
 *
 */
void Chart::resetZoom()
{
    customPlot->rescaleAxes(true);
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
 * @brief Get a data list depending on the current chart type (plot or
 * spectrum)
 *
 * @return const QList<qreal>&
 */
const QList<qreal> &Chart::getData() const
{
    return (chartType == ChartType::plot) ? plotDataContainer.getValues()
                                          : spectrumDataContainer.getValues();
}
