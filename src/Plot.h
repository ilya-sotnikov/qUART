#ifndef PLOT_H
#define PLOT_H

#include <QwtPlotCurve>
#include <QwtPlot>
#include <QwtPlotPanner>
#include <QwtPlotPicker>
#include <QwtPlotMarker>
#include <QwtPicker>
#include <QwtPickerClickPointMachine>
#include <QwtPlotMagnifier>

class Plot : public QwtPlot
{
    Q_OBJECT
public:
    explicit Plot(QWidget *parent = nullptr);
    ~Plot();
    void addRawData(QList<qreal> *receivedData);
    void updatePlot();
    void changeType();
    void clear();
    const QList<qreal> &getData() const { return *dataList; }
    QwtPlotCurve::CurveStyle getCurveStyle() const { return curve->style(); }

private:
    QwtPlotCurve *curve;
    QList<qreal> *dataList;
    QwtPlotMarker *marker;
    QwtPlotPanner *panner;
    QwtPickerClickPointMachine *plotPickerStateMachine;
    QwtPlotPicker *picker;
    QwtPlotMagnifier *magnifier;

public slots:
    void addData(QList<qreal> *receivedData);
    void resetZoom();

private slots:
    void updateSelected(const QPointF &point);

signals:
    void pointSelected(const QPointF point);
};

#endif // PLOT_H
