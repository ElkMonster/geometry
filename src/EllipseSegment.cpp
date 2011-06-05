#include "EllipseSegment.h"

namespace geom
{


EllipseSegment::EllipseSegment(const SegmentPoint& start, SegmentType type)
:   Segment(start, type)
{}

EllipseSegment::EllipseSegment(
    const SegmentPoint& start, Segment* next, SegmentType type)
:   Segment(start, next, type)
{}


Segment*
EllipseSegment::duplicate(const SegmentPoint& start, SegmentType type) const
{
    return new EllipseSegment(start, next_, type);
}


} // namespace geom

