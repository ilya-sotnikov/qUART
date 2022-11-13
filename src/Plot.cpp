#include "Plot.h"

#include <QwtScaleMap>
#include <QwtText>

Plot::Plot(QWidget *parent)
    : QwtPlot{ parent },
      curve{ new QwtPlotCurve{} },
      dataList{ new QList<qreal> },
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
    curve->setStyle(QwtPlotCurve::Sticks);
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
    delete dataList;
}

void Plot::addRawData(QList<qreal> *receivedData)
{
    dataList->append(*receivedData);
    updatePlot();
    receivedData->clear();
}

void Plot::updatePlot()
{
    curve->setRawSamples(dataList->constData(), dataList->size());
    replot();
}

void Plot::changeType()
{
    if (curve->style() == QwtPlotCurve::Sticks)
        curve->setStyle(QwtPlotCurve::Lines);
    else
        curve->setStyle(QwtPlotCurve::Sticks);
    clear();
}

void Plot::clear()
{
    dataList->clear();
    updatePlot();
}

void Plot::addData(QList<qreal> *receivedData)
{
    if (curve->style() == QwtPlotCurve::Lines) {
        dataList->append(*receivedData);
    } else {
        for (const auto &data : *receivedData) {
            if (data >= dataList->size())
                dataList->resize(data + 1);
            dataList->replace(data, dataList->at(data) + 1);
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