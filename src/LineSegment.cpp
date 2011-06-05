#include "LineSegment.h"

namespace geom
{


LineSegment::LineSegment(
    const SegmentPoint& start, SegmentType type)
:   Segment(start, type)
{}


LineSegment::LineSegment(
    const SegmentPoint& start, Segment* next, SegmentType type)
:   Segment(start, next, type)
{}


Segment*
LineSegment::duplicate(const SegmentPoint& start, SegmentType type) const
{
    return new LineSegment(start, next_, type);
}


} // namespace geom
