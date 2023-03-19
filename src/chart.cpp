#include "Chart.h"

#include <QwtScaleMap>
#include <QwtText>

/**
 * @brief Construct a new Plot object
 * 
 * @param parent 
 */
Chart::Chart(QWidget *parent)
    : QwtPlot{parent}
    , curve{new QwtPlotCurve{}}
    , dataPlot{new QList<qreal>}
    , dataSpectrum{new QList<qreal>}
    , marker{new QwtPlotMarker{}}
{
    [[maybe_unused]] auto panner{new QwtPlotPanner{canvas()}};

    auto plotPickerStateMachine{new QwtPickerClickPointMachine{}};

    marker->setLineStyle(QwtPlotMarker::VLine);
    marker->setLinePen(Qt::black, 0, Qt::SolidLine);
    marker->hide();
    marker->attach(this);
    marker->setValue(0.0, 0.0);

    curve->setStyle(QwtPlotCurve::Lines);
    curve->setPaintAttribute(QwtPlotCurve::FilterPoints);
    curve->setPen(Qt::black, 2);
    curve->attach(this);

    plotPickerStateMachine->setState(QwtPickerMachine::PointSelection);
    auto picker{new QwtPlotPicker(xBottom,
                                  yLeft,
                                  QwtPlotPicker::CrossRubberBand,
                                  QwtPlotPicker::AlwaysOn,
                                  canvas())};
    picker->setStateMachine(plotPickerStateMachine);

    auto magnifier{new QwtPlotMagnifier{canvas()}};
    magnifier->setMouseButton(Qt::MiddleButton);

    setAutoFillBackground(true);
    QPalette p{palette()};
    p.setColor(QPalette::Window, "white");
    setPalette(p);

    connect(picker, SIGNAL(selected(const QPointF &)), SLOT(updateSelected(const QPointF &)));
}

/**
 * @brief Destroy the Plot object and delete its data
 * 
 */
Chart::~Chart()
{
    delete dataPlot;
    delete dataSpectrum;
}

/**
 * @brief Update data on the chart
 * 
 */
void Chart::updateChart()
{
    if (chartType == Chart::plot)
        curve->setRawSamples(dataPlot->constData(), dataPlot->size());
    else if (chartType == Chart::spectrum)
        curve->setRawSamples(dataSpectrum->constData(), dataSpectrum->size());
    resetZoom();
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
        curve->setStyle(QwtPlotCurve::Lines);
        chartType = Chart::plot;
    } else if (chartType == Chart::plot) {
        curve->setStyle(QwtPlotCurve::Sticks);
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
    dataPlot->clear();
    dataSpectrum->clear();
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
    dataPlot->clear();
    dataSpectrum->clear();
    if (chartType == Chart::plot) {
        addData(rawData);
    } else if (chartType == Chart::spectrum) {
        dataSpectrum->append(*rawData);
        updateChart();
        rawData->clear();
    }
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
        dataPlot->append(*receivedData);

    if (appendToSpectrum) {
        for (const auto &data : *receivedData) {
            if (data >= dataSpectrum->size())
                dataSpectrum->resize(data + 1);
            dataSpectrum->replace(data, dataSpectrum->at(data) + 1);
        }
    }
    updateChart();
    receivedData->clear();
}

/**
 * @brief Enable autoscaling and replot
 * 
 */
void Chart::resetZoom()
{
    setAxisAutoScale(QwtPlot::xBottom);
    setAxisAutoScale(QwtPlot::yLeft);
    replot();
}

/**
 * @brief Return the selected data point from its coordinates and show marker at this position
 * 
 * @param point 
 */
void Chart::updateSelected(const QPointF &point)
{
    const auto dataList = (chartType == Chart::plot) ? dataPlot : dataSpectrum;

    qsizetype x = qRound(point.x());
    if (x < 0 || x >= dataList->size())
        return;

    auto selectedPoint{QPointF(x, dataList->at(x))};

    marker->setValue(selectedPoint);
    marker->show();
    replot();
    emit pointSelected(selectedPoint);
}

/**
 * @brief Hide the marker
 * 
 */
void Chart::hideMarker()
{
    marker->hide();
    replot();
}

/**
 * @brief Get chartType
 * 
 * @return Chart::ChartType 
 */
Chart::ChartType Chart::getChartType() const
{
    return chartType;
}

/**
 * @brief Get a data list depending on the current chart type (plot or spectrum)
 * 
 * @return const QList<qreal>& 
 */
const QList<qreal> &Chart::getData() const
{
    return (chartType == Chart::plot) ? *dataPlot : *dataSpectrum;
}
