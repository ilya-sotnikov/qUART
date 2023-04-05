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
}

/**
 * @brief Appends data to the plot and spectrum
 *
 * @param data Data to be appended
 * @param appendToPlot
 * @param appendToSpectrum
 */
void ChartDataContainer::append(const QList<qreal> &data, bool appendToPlot, bool appendToSpectrum)
{
    // the last spectrum values are recreated from the last plot values
    appendPlot(data, appendToPlot, appendToPlot || appendToSpectrum);
    appendSpectrum(data, appendToSpectrum, appendToSpectrum);
}

/**
 * @brief Appends data to the spectrum data container
 *
 * @param data Data to be appended
 * @param append
 * @param appendToLast
 *
 */
void ChartDataContainer::appendSpectrum(const QList<qreal> &data, bool append, bool appendToLast)
{
    if (!append && !appendToLast)
        return;

    if (append) {
        for (auto x : data) {
            auto it{ std::lower_bound(spectrumData->begin(), spectrumData->end(),
                                      QCPGraphData::fromSortKey(x),
                                      qcpLessThanSortKey<QCPGraphData>) };
            if ((it != spectrumData->end()) && (it->mainKey() == x))
                ++it->value;
            else
                spectrumData->add(QCPGraphData{ x, 1 });
        }
    }

    if (appendToLast) {
        if (lastPointsCount <= 0)
            return;

        spectrumDataLast->clear();

        for (auto point : *plotDataLast) {
            auto value{ point.mainValue() };
            auto it{ std::lower_bound(spectrumDataLast->begin(), spectrumDataLast->end(),
                                      QCPGraphData::fromSortKey(value),
                                      qcpLessThanSortKey<QCPGraphData>) };
            if ((it != spectrumDataLast->end()) && (it->mainKey() == value))
                ++it->value;
            else
                spectrumDataLast->add(QCPGraphData{ value, 1 });
        }
    }
}

void ChartDataContainer::setRawSpectrumData(const QCPGraphDataContainer &rawData)
{
    clear();
    spectrumData->set(rawData);
}

/**
 * @brief Appends data to the plot data container
 *
 * @param data Data to be appended
 * @param keys Append to these keys
 * @param append
 * @param appendToLast
 *
 */
void ChartDataContainer::appendPlot(const QList<qreal> &data, bool append, bool appendToLast)
{
    if (!append && !appendToLast)
        return;

    const auto dataSize{ data.size() };
    static qsizetype currentDataSize{ 0 };
    for (qsizetype i{ 0 }; i < dataSize; ++i) {
        auto key{ static_cast<double>(i + currentDataSize) };
        auto value{ data.at(i) };

        if (append)
            plotData->add(QCPGraphData{ key, value });

        if (appendToLast)
            plotDataLast->add(QCPGraphData{ key, value });

        if (append || appendToLast)
            ++currentDataSize;
    }

    if (lastPointsCount <= 0)
        return;

    qsizetype deleteCnt{ plotDataLast->size() - lastPointsCount };
    if (deleteCnt > 0) {
        while (deleteCnt > 0) {
            auto it{ plotDataLast->begin() };
            plotDataLast->remove(it->sortKey());
            --deleteCnt;
        }
    }
}
