#include "Plot.h"

#include <QwtScaleMap>
#include <QwtText>

Plot::Plot(QWidget *parent)
    : QwtPlot{ parent },
      curve{ new QwtPlotCurve{} },
      dataPlot{ new QList<qreal> },
      dataSpectrum{ new QList<qreal> },
      marker{ new QwtPlotMarker{} },
      panner{ new QwtPlotPanner{ canvas() } },
      plotPickerStateMachine{ new QwtPickerClickPointMachine{} },
      picker{ new QwtPlotPicker(xBottom, yLeft, QwtPlotPicker::CrossRubberBand,
                                QwtPlotPicker::AlwaysOn, canvas()) },
      magnifier{ new QwtPlotMagnifier{ canvas() } }
{
    marker->setValue(0.0, 0.0);
    marker->setLineStyle(QwtPlotMarker::VLine);
    marker->setLinePen(Qt::black, 0, Qt::SolidLine);
    marker->hide();
    marker->attach(this);
    curve->setStyle(QwtPlotCurve::Lines);
    curve->setPaintAttribute(QwtPlotCurve::FilterPoints);
    curve->setPen(Qt::black, 2);
    curve->attach(this);
    plotPickerStateMachine->setState(QwtPickerMachine::PointSelection);
    picker->setStateMachine(plotPickerStateMachine);
    magnifier->setMouseButton(Qt::MiddleButton);
    setAutoFillBackground(true);
    QPalette p{ palette() };
    p.setColor(QPalette::Window, "white");
    setPalette(p);

    connect(picker, SIGNAL(selected(const QPointF &)), SLOT(updateSelected(const QPointF &)));
}

Plot::~Plot()
{
    delete dataPlot;
    delete dataSpectrum;
}

void Plot::updatePlot()
{
    if (chartType == Plot::plot)
        curve->setRawSamples(dataPlot->constData(), dataPlot->size());
    else if (chartType == Plot::spectrum)
        curve->setRawSamples(dataSpectrum->constData(), dataSpectrum->size());
    resetZoom();
}

void Plot::changeType()
{
    if (chartType == Plot::spectrum) {
        curve->setStyle(QwtPlotCurve::Lines);
        chartType = Plot::plot;
    } else if (chartType == Plot::plot) {
        curve->setStyle(QwtPlotCurve::Sticks);
        chartType = Plot::spectrum;
    }
    updatePlot();
}

void Plot::clear()
{
    dataPlot->clear();
    dataSpectrum->clear();
    updatePlot();
}

void Plot::addRawData(QList<qreal> *rawData)
{
    dataPlot->clear();
    dataSpectrum->clear();
    if (chartType == Plot::plot) {
        addData(rawData);
    } else if (chartType == Plot::spectrum) {
        dataSpectrum->append(*rawData);
        updatePlot();
        rawData->clear();
    }
}

void Plot::addData(QList<qreal> *receivedData)
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
    updatePlot();
    receivedData->clear();
}

void Plot::resetZoom()
{
    setAxisAutoScale(QwtPlot::xBottom);
    setAxisAutoScale(QwtPlot::yLeft);
    replot();
}

void Plot::updateSelected(const QPointF &point)
{
    const auto dataList = (chartType == Plot::plot) ? dataPlot : dataSpectrum;

    qsizetype x = qRound(point.x());
    if (x < 0 || x >= dataList->size())
        return;

    auto selectedPoint{ QPointF(x, dataList->at(x)) };

    marker->setValue(selectedPoint);
    marker->show();
    replot();
    emit pointSelected(selectedPoint);
}

void Plot::hideMarker()
{
    marker->hide();
    replot();
}
Plot::ChartType Plot::getChartType() const
{
    return chartType;
}

const QList<qreal> &Plot::getData() const
{
    return (chartType == Plot::plot) ? *dataPlot : *dataSpectrum;
}
