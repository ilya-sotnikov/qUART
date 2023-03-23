#ifndef CHARTDATACONTAINER_H
#define CHARTDATACONTAINER_H

#include <qlist.h>

/**
 * @brief An abstract data container for plot and spectrum
 */
class ChartDataContainer
{
public:
    virtual void append(const QList<qreal> &data) = 0;
    void setRawData(const QList<qreal> &rawData);
    void clear();

    const auto &getKeys() const { return keys; }
    const auto &getValues() const { return values; }

protected:
    QList<qreal> keys;
    QList<qreal> values;
};

/**
 * @brief A data container for a plot
 */
class PlotDataContainer : public ChartDataContainer
{
public:
    virtual void append(const QList<qreal> &data) override;
};

/**
 * @brief A data container for a spectrum
 */
class SpectrumDataContainer : public ChartDataContainer
{
public:
    SpectrumDataContainer();
    virtual void append(const QList<qreal> &data) override;
};

#endif // CHARTDATACONTAINER_H
