#include "Plot.h"

#include <QwtSymbol>

Plot::Plot(QWidget* parent) :
    QwtPlot{parent},
    curve{new QwtPlotCurve},
    dataList{new QList<qreal>},
    plotPanner{new QwtPlotPanner{canvas()}},
    plotPicker{new QwtPlotPicker(xBottom, yLeft, QwtPicker::CrossRubberBand, QwtPicker::AlwaysOn, canvas())},
    plotMagnifier{new QwtPlotMagnifier{canvas()}}
{
    curve->setStyle(QwtPlotCurve::Sticks);
    curve->setPaintAttribute(QwtPlotCurve::FilterPoints);
    curve->setPen(Qt::black, 2);
    curve->attach(this);
    plotMagnifier->setMouseButton(Qt::MiddleButton);
    setAutoFillBackground(true);
    QPalette p {palette()};
    p.setColor(QPalette::Window, "white");
    setPalette(p);
}

Plot::~Plot()
{
    delete dataList;
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

const QList<qreal>& Plot::getData() const
{
    return *dataList;
}

QwtPlotCurve::CurveStyle Plot::getCurveStyle() const
{
    return curve->style();
}

void Plot::addData(QList<qreal>* receivedData)
{
    if (curve->style() == QwtPlotCurve::Lines) {
        dataList->append(*receivedData);
    } else {
        for (const auto& data : *receivedData) {
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
