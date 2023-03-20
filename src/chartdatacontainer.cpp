#include "chartdatacontainer.h"

#include <QDebug>

void ChartDataContainer::clear()
{
    keys.clear();
    values.clear();
}

SpectrumDataContainer::SpectrumDataContainer()
{
    keys.append(0);
    values.append(0);
}

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

void PlotDataContainer::append(const QList<qreal> &data)
{
    auto i{ keys.size() };
    for (const auto dataY : data) {
        keys.append(i++);
        values.append(dataY);
    }
}
