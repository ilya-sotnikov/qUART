#ifndef CHART_H
#define CHART_H

#include <QwtPicker>
#include <QwtPickerClickPointMachine>
#include <QwtPlot>
#include <QwtPlotCurve>
#include <QwtPlotMagnifier>
#include <QwtPlotMarker>
#include <QwtPlotPanner>
#include <QwtPlotPicker>

class Chart : public QwtPlot {
  Q_OBJECT
public:
  explicit Chart(QWidget *parent = nullptr);
  ~Chart();
  void updateChart();
  void changeType();
  void clear();
  void addRawData(QList<qreal> *rawData);
  const QList<qreal> &getData() const;

  enum ChartType { plot, spectrum };
  Q_ENUM(ChartType)
  Chart::ChartType getChartType() const;

  bool appendToPlot{true};
  bool appendToSpectrum{true};

private:
  Chart::ChartType chartType{Chart::plot};
  QwtPlotCurve *curve;
  QList<qreal> *dataPlot;
  QList<qreal> *dataSpectrum;
  QwtPlotMarker *marker;

public slots:
  void addData(QList<qreal> *receivedData);
  void resetZoom();
  void hideMarker();

private slots:
  void updateSelected(const QPointF &point);

signals:
  void pointSelected(const QPointF point);
};

#endif // CHART_H
