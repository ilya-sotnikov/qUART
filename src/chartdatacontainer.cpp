#include "chartdatacontainer.h"

/**
 * @brief Clears all data
 *
 */
void ChartDataContainer::clear()
{
    plotData.keys.clear();
    plotData.keys.squeeze();
    plotData.values.clear();
    plotData.values.squeeze();
    plotDataLast.keys.clear();
    plotDataLast.keys.squeeze();
    plotDataLast.values.clear();
    plotDataLast.values.squeeze();
    spectrumData.keys.clear();
    spectrumData.keys.squeeze();
    spectrumData.values.clear();
    spectrumData.values.squeeze();
    spectrumDataLast.keys.clear();
    spectrumDataLast.keys.squeeze();
    spectrumDataLast.values.clear();
    spectrumDataLast.values.squeeze();
    spectrumMap.clear();
}

/**
 * @brief Returns the last n points from the plot data
 *
 */
const ChartDataContainer::PlotData &ChartDataContainer::getPlot(qsizetype n)
{
    const auto size{ plotData.keys.size() };

    if (n > size || n < 0)
        return plotData;

    plotDataLast = { plotData.keys.sliced(size - n), plotData.values.sliced(size - n) };

    return plotDataLast;
};

/**
 * @brief Returns the last n points from the spectrum data
 *
 */
const ChartDataContainer::SpectrumData &ChartDataContainer::getSpectrum(qsizetype n)
{
    spectrumDataLast.keys.clear();
    spectrumDataLast.values.clear();

    const auto size{ plotData.keys.size() };

    if (n > size || n < 0)
        return spectrumData;

    appendSpectrum(getPlot(n).values, spectrumDataLast.keys, spectrumDataLast.values);

    return spectrumDataLast;
};

/**
 * @brief Appends data to the plot and spectrum
 *
 * @param data Data to be appended
 * @param appendToPlot
 * @param appendToSpectrum
 */
void ChartDataContainer::append(const QList<qreal> &data, bool appendToPlot, bool appendToSpectrum)
{
    if (appendToPlot)
        appendPlot(data, plotData.keys, plotData.values);
    if (appendToSpectrum)
        appendSpectrum(data, spectrumData.keys, spectrumData.values);
}

/**
 * @brief Appends data to the spectrum data container
 *
 * @param data Data to be appended
 * @param keys Append to these keys
 * @param values Append to these values
 */
void ChartDataContainer::appendSpectrum(const QList<qreal> &data, QList<qreal> &keys,
                                        QList<qreal> &values)
{
    for (auto x : data)
        spectrumMap.insert(x, spectrumMap.value(x, 0) + 1);

    keys = spectrumMap.keys();
    values = spectrumMap.values();
}

void ChartDataContainer::setRawSpectrumData(const QMap<qreal, qreal> rawData)
{
    spectrumMap = rawData;
    spectrumData.keys = spectrumMap.keys();
    spectrumData.values = spectrumMap.values();
}

/**
 * @brief Appends data to the plot data container
 *
 * @param data Data to be appended
 * @param keys Append to these keys
 * @param values Append to these values
 */
void ChartDataContainer::appendPlot(const QList<qreal> &data, QList<qreal> &keys,
                                    QList<qreal> &values)
{
    const auto dataSize{ data.size() };
    const auto currentDataSize{ keys.size() };
    for (auto i{ currentDataSize }; i < currentDataSize + dataSize; ++i)
        keys.append(i);

    values.append(data);
}
