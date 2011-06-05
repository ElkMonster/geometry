#ifndef SEGMENTEDSHAPE_H_
#define SEGMENTEDSHAPE_H_

#include "Shape.h"
#include "Segment.h"
#include "SegmentPointVector.h"
#include <ostream>

namespace geom
{


class SegmentedShape
{

public:

    SegmentedShape(const Shape* shape, Segment* start);

    ~SegmentedShape();

public:

    void addIntersections(SegmentPointVector& p);

    friend std::ostream& operator<<(std::ostream& out, const SegmentedShape& s);

private:

    const Shape* const shape_;

    Segment* start_;

};


} // namespace geom

#endif // SEGMENTEDSHAPE_H_
