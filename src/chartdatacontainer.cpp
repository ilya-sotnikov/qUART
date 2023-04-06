#include "chartdatacontainer.h"

#include <algorithm>

/**
 * @brief Clears all data
 *
 */
void ChartDataContainer::clear()
{
    plotData->clear();
    plotDataLast->clear();
    spectrumData->clear();
    spectrumDataLast->clear();
    currentPlotDataCount = 0;
}

/**
 * @brief Appends data to the plot and spectrum
 *
 * Note that the last spectrum values are recreated from the last plot values so if
 * appendSpectrum == true then the data is always added to the plotDataLast
 *
 * @param data Data to be appended
 * @param appendToPlot
 * @param appendToSpectrum
 *
 */
void ChartDataContainer::append(const QList<qreal> &data, const bool appendToPlot,
                                const bool appendToSpectrum)
{
    appendPlot(data, appendToPlot, appendToPlot || appendToSpectrum);
    appendSpectrum(data, appendToSpectrum, appendToSpectrum);
}

/**
 * @brief Appends a value to the spectrum container
 *
 * @param value The value to be appended
 * @param spectrumContainer
 *
 */
void ChartDataContainer::appendSpectrumValue(const double value,
                                             QCPGraphDataContainer &spectrumContainer) const
{
    const auto it{ std::lower_bound(spectrumContainer.begin(), spectrumContainer.end(),
                                    QCPGraphData::fromSortKey(value),
                                    qcpLessThanSortKey<QCPGraphData>) };
    if ((it != spectrumContainer.end()) && (it->mainKey() == value))
        ++it->value;
    else
        spectrumContainer.add(QCPGraphData{ value, 1 });
}

/**
 * @brief Appends data to the spectrum data container
 *
 * @param data Data to be appended
 * @param append
 * @param appendToLast
 *
 */
void ChartDataContainer::appendSpectrum(const QList<qreal> &data, const bool append,
                                        const bool appendToLast) const
{
    if (!append && !appendToLast)
        return;

    if (append) {
        for (const auto x : data)
            appendSpectrumValue(x, *spectrumData);
    }

    if (appendToLast) {
        if (lastPointsCount <= 0)
            return;

        spectrumDataLast->clear();

        for (const auto point : *plotDataLast)
            appendSpectrumValue(point.mainValue(), *spectrumDataLast);
    }
}

/**
 * @brief Sets spectrumData to raw data, useful when loading data from a file
 *
 * @param rawData
 *
 */
void ChartDataContainer::setRawSpectrumData(const QCPGraphDataContainer &rawData)
{
    clear();
    spectrumData->set(rawData);
}

/**
 * @brief Appends data to the plot data container
 *
 * @param data Data to be appended
 * @param append
 * @param appendToLast
 *
 */
void ChartDataContainer::appendPlot(const QList<qreal> &data, const bool append,
                                    const bool appendToLast)
{
    if (!append && !appendToLast)
        return;

    const auto dataSize{ data.size() };
    for (qsizetype i{ 0 }; i < dataSize; ++i) {
        const auto key{ static_cast<double>(i + currentPlotDataCount) };
        const auto value{ data.at(i) };

        if (append)
            plotData->add(QCPGraphData{ key, value });

        if (appendToLast)
            plotDataLast->add(QCPGraphData{ key, value });

        if (append || appendToLast)
            ++currentPlotDataCount;
    }

    if (lastPointsCount <= 0)
        return;

    qsizetype deleteCnt{ plotDataLast->size() - lastPointsCount };
    if (deleteCnt > 0) {
        while (deleteCnt-- > 0)
            plotDataLast->remove(plotDataLast->begin()->sortKey());
    }
}
