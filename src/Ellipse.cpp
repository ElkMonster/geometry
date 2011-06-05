#include "Ellipse.h"

#include "Rectangle.h"
#include "Triangle.h"
#include "SegmentPoint.h"
#include "SegmentedShape.h"
#include "EllipseSegment.h"
#include "Limits.h"
#include "Helpers.h"
#include <cmath>

namespace geom
{


Ellipse::Ellipse()
:   GenericEllipse(),
    Shape(TEllipse)
{}


Ellipse::Ellipse(const Point2D& center, const Point2D& radius)
:   GenericEllipse(center, radius),
    Shape(TEllipse)
{}


Ellipse::~Ellipse()
{}


void
Ellipse::performCleaning() const
{
    // Nothing to do here
}

void
Ellipse::calculateBoundingBox(GenericRect& bb) const
{
    bb.p1(center_ - radius_);
    bb.p2(center_ + radius_);
}


void
Ellipse::moveBy(const Point2D& delta)
{
    GenericEllipse::moveBy(delta);
    dirty_ = true;
}


bool
Ellipse::isIntersectedByRectangle(
    const Rectangle* r, SegmentPointVector& isecPoints, int& isecCount) const
{
    return isIntersectedByLineBasedShape(r, isecPoints, isecCount);
}


bool
Ellipse::isIntersectedByTriangle(
    const Triangle* t, SegmentPointVector& isecPoints, int& isecCount) const
{
    return isIntersectedByLineBasedShape(t, isecPoints, isecCount);
}


bool
Ellipse::isIntersectedByEllipse(
    const Ellipse* e, SegmentPointVector& isecPoints, int& isecCount) const
{
    return GenericEllipse::isIntersectedBy(e, isecPoints, isecCount);
}


SegmentedShape*
Ellipse::toSegmentedShape() const
{
    CLEAN_IF_DIRTY(this);

    SegmentPoint sp[] = {
        SegmentPoint(center_ + Point2D(0, radius_.y), 0.f, &quadrant_[0]),
        SegmentPoint(center_ + Point2D(radius_.x, 0), 0.f, &quadrant_[1]),
        SegmentPoint(center_ - Point2D(0, radius_.y), 0.f, &quadrant_[2]),
        SegmentPoint(center_ - Point2D(radius_.x, 0), 0.f, &quadrant_[3])
    };

    EllipseSegment* q4 = new EllipseSegment(sp[3], Segment::Positive);
    EllipseSegment* q3 = new EllipseSegment(sp[2], q4, Segment::Positive);
    EllipseSegment* q2 = new EllipseSegment(sp[1], q3, Segment::Positive);
    EllipseSegment* q1 = new EllipseSegment(sp[0], q2, Segment::Positive);
    q4->next(q1);

    return new SegmentedShape(this, q1);
}


bool
Ellipse::isIntersectedByLineBasedShape(
    const LineBasedShape* s,
    SegmentPointVector& isecPoints,
    int& isecCount) const
{
    if (!bb().isIntersectedByRect(s->bb()))
    {
        isecCount = 0;
        return false;
    }

    return GenericEllipse::isIntersectedBy(
        s->lines(), s->getNumLines(), isecPoints, isecCount);
}


} // namespace geom
