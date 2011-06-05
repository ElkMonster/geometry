#ifndef LINESEGMENT_H_
#define LINESEGMENT_H_

#include "Segment.h"
#include "GenericLine.h"

namespace geom
{


class LineSegment : public Segment
{

public:

    LineSegment(const SegmentPoint& start, SegmentType type);

    LineSegment(const SegmentPoint& start, Segment* next, SegmentType type);

protected:

    Segment* duplicate(const SegmentPoint& start, SegmentType type) const;


};


} // namespace geom

#endif // LINESEGMENT_H_
