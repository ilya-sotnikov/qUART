#ifndef CHARTDATACONTAINER_H
#define CHARTDATACONTAINER_H

#include <qlist.h>
#include <qmap.h>

/**
 * @brief A data container for a plot and a spectrum
 */
class ChartDataContainer
{
public:
    void append(const QList<qreal> &data, bool appendToPlot, bool appendToSpectrum);
    void setRawSpectrumData(const QMap<qreal, qreal> rawData);
    void clear();

    struct PlotData
    {
        QList<qreal> keys;
        QList<qreal> values;
    };

    struct SpectrumData
    {
        QList<qreal> keys;
        QList<qreal> values;
    };

    const auto &getPlot() const { return plotData; }
    const auto &getSpectrum() const { return spectrumData; }
    const auto &getSpectrumMap() const { return spectrumMap; }
    const PlotData &getPlot(qsizetype n);
    const SpectrumData &getSpectrum(qsizetype n);

private:
    void appendPlot(const QList<qreal> &data, QList<qreal> &keys, QList<qreal> &values);
    void appendSpectrum(const QList<qreal> &data, QList<qreal> &keys, QList<qreal> &values);

    PlotData plotData;
    PlotData plotDataLast;
    SpectrumData spectrumData;
    SpectrumData spectrumDataLast;
    QMap<qreal, qreal> spectrumMap;
};

#endif // CHARTDATACONTAINER_H
