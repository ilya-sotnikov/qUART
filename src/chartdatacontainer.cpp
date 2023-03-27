#include "chartdatacontainer.h"

/**
 * @brief Sets raw spectrum data without any modifications
 *
 * @param rawData
 */
void ChartDataContainer::setRawSpectrumData(const QList<qreal> &rawData)
{
    clear();

    spectrumData.values.append(rawData);

    auto rawDataSize{ rawData.size() };
    for (decltype(rawDataSize) i{ 0 }; i < rawDataSize; ++i)
        spectrumData.keys.append(i);
}

/**
 * @brief Clears all data
 *
 */
void ChartDataContainer::clear()
{
    plotData.keys.clear();
    plotData.values.clear();
    plotDataLast.keys.clear();
    plotDataLast.values.clear();
    spectrumData.keys.clear();
    spectrumData.values.clear();
    spectrumData.keys.clear();
    spectrumData.values.clear();
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
    QList<quint64> dataQuint64;
    quint64 maxData{ 0 };
    for (auto x : data) {
        if (x >= 0) {
            if (x > maxData)
                maxData = x;
            dataQuint64.append(static_cast<quint64>(x));
        }
    }

    auto spectrumSize{ static_cast<quint64>(values.size()) };

    if (maxData >= spectrumSize) {
        auto sizeToFill{ maxData - spectrumSize + 1 };
        QList<qreal> tempY;

        for (auto i{ spectrumSize }; i < spectrumSize + sizeToFill; ++i)
            keys.append(i);

        tempY.fill(0, sizeToFill);
        values.append(tempY);
    }

    for (const quint64 dataY : dataQuint64)
        ++values[dataY];
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
