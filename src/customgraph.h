#ifndef CUSTOMGRAPH_H
#define CUSTOMGRAPH_H

#include "qcustomplot.h"

class CustomGraph : public QCPGraph
{
protected:
    using QCPGraph::QCPGraph;
    double pointDistance(const QPointF &pixelPoint,
                         QCPGraphDataContainer::const_iterator &closestData) const;
    virtual double selectTest(const QPointF &pos, bool onlySelectable,
                              QVariant *details = nullptr) const Q_DECL_OVERRIDE;
};

#endif // CUSTOMGRAPH_H
