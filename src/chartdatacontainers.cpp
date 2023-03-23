#include "chartdatacontainers.h"
#include <qglobal.h>

/**
 * @brief Sets raw data without any modifications
 *
 * @param rawData
 */
void ChartDataContainer::setRawData(const QList<qreal> &rawData)
{
    clear();

    values.append(rawData);

    auto rawDataSize{ rawData.size() };
    for (decltype(rawDataSize) i{ 0 }; i < rawDataSize; ++i)
        keys.append(i);
}

/**
 * @brief Clears keys and values
 *
 */
void ChartDataContainer::clear()
{
    keys.clear();
    values.clear();
}

/**
 * @brief Adds 0 to keys and values to avoid an unnecessary check every time when calling append()
 *
 */
SpectrumDataContainer::SpectrumDataContainer()
{
    keys.append(0);
    values.append(0);
}

/**
 * @brief Appends data to the spectrum data container
 *
 * @param data
 */
void SpectrumDataContainer::append(const QList<qreal> &data)
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

    if (maxData > spectrumSize) {
        auto sizeToFill{ maxData - spectrumSize + 1 };
        QList<qreal> tempX;
        QList<qreal> tempY;

        for (auto i{ spectrumSize }; i < spectrumSize + sizeToFill; ++i)
            keys.append(i);

        tempY.fill(0, sizeToFill);
        keys.append(tempX);
        values.append(tempY);
    }

    for (const quint64 dataY : dataQuint64)
        ++values[dataY];
}

/**
 * @brief Appends data to the plot data container
 *
 * @param data
 */
void PlotDataContainer::append(const QList<qreal> &data)
{
    const auto dataSize{ data.size() };
    const auto currentDataSize{ keys.size() };
    for (auto i{ currentDataSize }; i < currentDataSize + dataSize; ++i)
        keys.append(i);

    values.append(data);
}
