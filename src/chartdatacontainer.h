#ifndef CHARTDATACONTAINER_H
#define CHARTDATACONTAINER_H

#include <QList>

class ChartDataContainer
{
public:
    virtual void append(const QList<qreal> &data) = 0;
    void clear();

    const auto &getKeys() const { return keys; }
    const auto &getValues() const { return values; }

protected:
    QList<qreal> keys;
    QList<qreal> values;
};

class PlotDataContainer : public ChartDataContainer
{
public:
    virtual void append(const QList<qreal> &data) override;
};

class SpectrumDataContainer : public ChartDataContainer
{
public:
    SpectrumDataContainer();
    virtual void append(const QList<qreal> &data) override;
};

#endif // CHARTDATACONTAINER_H
