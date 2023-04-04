#ifndef CHARTDATACONTAINER_H
#define CHARTDATACONTAINER_H

#include "qcustomplot.h"
#include <qlist.h>

/**
 * @brief A data container for a plot and a spectrum
 */
class ChartDataContainer
{
public:
    void append(const QList<qreal> &data, bool appendToPlot, bool appendToSpectrum);
    void setRawSpectrumData(const QCPGraphDataContainer &rawData);
    void clear();

    const auto &getPlot() const { return plotData; }
    const auto &getSpectrum() const { return spectrumData; }
    auto getLastPointsCount() const { return lastPointsCount; };
    auto setLastPointsCount(qsizetype n) { lastPointsCount = n; };
    const QSharedPointer<QCPGraphDataContainer> &getPlotLast() const
    {
        return (lastPointsCount < 0) ? plotData : plotDataLast;
    };
    const QSharedPointer<QCPGraphDataContainer> &getSpectrumLast() const
    {
        return (lastPointsCount < 0) ? spectrumData : spectrumDataLast;
    };

private:
    void appendPlot(const QList<qreal> &data);
    void appendSpectrum(const QList<qreal> &data);

    qsizetype lastPointsCount{ -1 };
    QSharedPointer<QCPGraphDataContainer> plotData{
        QSharedPointer<QCPGraphDataContainer>::create()
    };
    QSharedPointer<QCPGraphDataContainer> plotDataLast{
        QSharedPointer<QCPGraphDataContainer>::create()
    };
    QSharedPointer<QCPGraphDataContainer> spectrumData{
        QSharedPointer<QCPGraphDataContainer>::create()
    };
    QSharedPointer<QCPGraphDataContainer> spectrumDataLast{
        QSharedPointer<QCPGraphDataContainer>::create()
    };
};

#endif // CHARTDATACONTAINER_H
