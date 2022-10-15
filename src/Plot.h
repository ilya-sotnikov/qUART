#ifndef PLOT_H
#define PLOT_H

#include <QwtPlotCurve>
#include <QwtPlot>
#include <QwtPlotPanner>
#include <QwtPlotPicker>
#include <QwtPlotMagnifier>

class Plot : public QwtPlot
{
    Q_OBJECT
public:
    explicit Plot(QWidget* parent = nullptr);
    ~Plot();
    void addRawData(QList<qreal>* receivedData);
    void updatePlot();
    void changeType();
    void clear();
    const QList<qreal>& getData() const;
    QwtPlotCurve::CurveStyle getCurveStyle() const;

private:
    QwtPlotCurve* curve {nullptr};
    QList<qreal>* dataList {nullptr};
    QwtPlotPanner *plotPanner {nullptr};
    QwtPlotPicker *plotPicker {nullptr};
    QwtPlotMagnifier *plotMagnifier {nullptr};


public slots:
    void addData(QList<qreal>* receivedData);
    void resetZoom();
};

#endif // PLOT_H
