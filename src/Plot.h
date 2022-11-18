#ifndef PLOT_H
#define PLOT_H

#include <QwtPicker>
#include <QwtPickerClickPointMachine>
#include <QwtPlot>
#include <QwtPlotCurve>
#include <QwtPlotMagnifier>
#include <QwtPlotMarker>
#include <QwtPlotPanner>
#include <QwtPlotPicker>

class Plot : public QwtPlot
{
    Q_OBJECT
public:
    explicit Plot(QWidget *parent = nullptr);
    ~Plot();
    void updatePlot();
    void changeType();
    void clear();
    void addRawData(QList<qreal> *rawData);
    const QList<qreal> &getData() const;

    enum ChartType { plot, spectrum };
    Q_ENUM(ChartType)
    Plot::ChartType getChartType() const;

    bool appendToPlot{true};
    bool appendToSpectrum{true};

private:
    Plot::ChartType chartType{Plot::plot};
    QwtPlotCurve *curve;
    QList<qreal> *dataPlot;
    QList<qreal> *dataSpectrum;
    QwtPlotMarker *marker;
    QwtPlotPanner *panner;
    QwtPickerClickPointMachine *plotPickerStateMachine;
    QwtPlotPicker *picker;
    QwtPlotMagnifier *magnifier;

public slots:
    void addData(QList<qreal> *receivedData);
    void resetZoom();
    void hideMarker();

private slots:
    void updateSelected(const QPointF &point);

signals:
    void pointSelected(const QPointF point);
};

#endif // PLOT_H
