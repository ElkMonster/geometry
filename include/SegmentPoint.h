#ifndef SEGMENTPOINT_H_
#define SEGMENTPOINT_H_

#include <Point2D.h>
#include "GenericShapeElement.h"

namespace geom
{


// Forward declaration
//class GenericShapeElement;


class SegmentPoint : public Point2D
{

public:

    SegmentPoint(float x, float y, float t, const GenericShapeElement* parent);

    SegmentPoint(const Point2D& p, float t, const GenericShapeElement* parent);

    SegmentPoint(
        const Point2D& p,
        float t,
        const GenericShapeElement* parent,
        float t2,
        const GenericShapeElement* parent2);

    SegmentPoint(const SegmentPoint* other);

    ~SegmentPoint();

    float t;

    const GenericShapeElement* parent;

    float t2;

    const GenericShapeElement* parent2;

};


} // namespace geom

#endif // SEGMENTPOINT_H_
