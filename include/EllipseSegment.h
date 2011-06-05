#ifndef ELLIPSESEGMENT_H_
#define ELLIPSESEGMENT_H_

#include "Segment.h"
#include "Ellipse.h"
#include "GenericArc.h"

namespace geom
{


class EllipseSegment : public Segment
{

public:

    EllipseSegment(const SegmentPoint& start, SegmentType type);

    EllipseSegment(const SegmentPoint& start, Segment* next, SegmentType type);

protected:

    Segment* duplicate(const SegmentPoint& start, SegmentType type) const;

};


} // namespace geom

#endif // ELLIPSESEGMENT_H_
