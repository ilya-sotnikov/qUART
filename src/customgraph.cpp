#include "customgraph.h"
#include "qcustomplot.h"
#include <qnamespace.h>

double CustomGraph::pointDistance(const QPointF &pixelPoint,
                                  QCPGraphDataContainer::const_iterator &closestData) const
{
    closestData = mDataContainer->constEnd();
    if (mDataContainer->isEmpty())
        return -1.0;
    if (mLineStyle == lsNone && mScatterStyle.isNone())
        return -1.0;

    // calculate minimum distances to graph data points and find closestData iterator:
    double minDistSqr = (std::numeric_limits<double>::max)();
    // determine which key range comes into question, taking selection tolerance around pos into
    // account:
    double posKeyMin, posKeyMax, dummy;
    pixelsToCoords(
            pixelPoint
                    - QPointF(mParentPlot->selectionTolerance(), mParentPlot->selectionTolerance()),
            posKeyMin, dummy);
    pixelsToCoords(
            pixelPoint
                    + QPointF(mParentPlot->selectionTolerance(), mParentPlot->selectionTolerance()),
            posKeyMax, dummy);
    if (posKeyMin > posKeyMax)
        qSwap(posKeyMin, posKeyMax);
    // iterate over found data points and then choose the one with the shortest distance to pos:
    QCPGraphDataContainer::const_iterator begin = mDataContainer->findBegin(posKeyMin, true);
    QCPGraphDataContainer::const_iterator end = mDataContainer->findEnd(posKeyMax, true);
    for (QCPGraphDataContainer::const_iterator it = begin; it != end; ++it) {
        // const double currentDistSqr =
        //         QCPVector2D(coordsToPixels(it->key, it->value) - pixelPoint).lengthSquared();
        const double currentDistSqr = qPow(keyAxis()->coordToPixel(it->key) - pixelPoint.x(), 2);
        // const double currentDistSqr =
        //         qPow(coordsToPixels(it->key, it->value).x() - pixelPoint.x(), 2);
        if (currentDistSqr < minDistSqr) {
            minDistSqr = currentDistSqr;
            closestData = it;
        }
    }

    // calculate distance to graph line if there is one (if so, will probably be smaller than
    // distance to closest data point):
    // if (mLineStyle != lsNone) {
    //     // line displayed, calculate distance to line segments:
    //     QVector<QPointF> lineData;
    //     getLines(&lineData,
    //              QCPDataRange(0, dataCount())); // don't limit data range further since with
    //              sharp
    //                                             // data spikes, line segments may be closer to
    //                                             test
    //                                             // point than segments with closer key
    // coordinate
    //     QCPVector2D p(pixelPoint);
    //     const int step = mLineStyle == lsImpulse ? 2 : 1;
    //     for (int i = 0; i < lineData.size() - 1; i += step) {
    //         const double currentDistSqr =
    //                 p.distanceSquaredToLine(lineData.at(i), lineData.at(i + 1));
    //         if (currentDistSqr < minDistSqr)
    //             minDistSqr = currentDistSqr;
    //     }
    // }

    return qSqrt(minDistSqr);
}

double CustomGraph::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    if ((onlySelectable && mSelectable == QCP::stNone) || mDataContainer->isEmpty())
        return -1;
    if (!mKeyAxis || !mValueAxis)
        return -1;

    if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint())
        || mParentPlot->interactions().testFlag(QCP::iSelectPlottablesBeyondAxisRect)) {
        QCPGraphDataContainer::const_iterator closestDataPoint = mDataContainer->constEnd();
        double result = pointDistance(pos, closestDataPoint);
        if (details) {
            int pointIndex = int(closestDataPoint - mDataContainer->constBegin());
            details->setValue(QCPDataSelection(QCPDataRange(pointIndex, pointIndex + 1)));
        }
        return result;
    } else
        return -1;
}
