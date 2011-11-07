#include "Rectangle.h"

#include "SegmentedShape.h"
#include "LineSegment.h"

namespace geom
{

Rectangle::Rectangle()
:   LineBasedShape(TRectangle)
{
    lines_ = r_.lines_;
}


Rectangle::Rectangle(const Point2D& p1, const Point2D& p2)
:   LineBasedShape(TRectangle),
    r_(GenericRect(p1, p2))
{
    lines_ = r_.lines_;
}


Rectangle::~Rectangle()
{}


void
Rectangle::p1p2(const Point2D& p1, const Point2D& p2)
{
    r_.p1(p1);
    r_.p2(p2);
    dirty_ = true;
}


void
Rectangle::performCleaning() const
{
    r_.makeClean();
}


void
Rectangle::calculateBoundingBox(GenericRect& bb) const
{
    bb = r_;
}


void
Rectangle::moveBy(const Point2D& delta)
{
    r_.moveBy(delta);
    dirty_ = true;
}


SegmentedShape*
Rectangle::toSegmentedShape() const
{
    CLEAN_IF_DIRTY(this);

    SegmentPoint sp[] =
    {
        SegmentPoint(r_.p1_, 0.f, &lines_[0]),
        SegmentPoint(r_.topRight_, 0.f, &lines_[1]),
        SegmentPoint(r_.p2_, 0.f, &lines_[2]),
        SegmentPoint(r_.bottomLeft_, 0.f, &lines_[3])
    };

    LineSegment* end = new LineSegment(sp[3], Segment::Positive);
    LineSegment* middle2 = new LineSegment(sp[2], end, Segment::Positive);
    LineSegment* middle1 = new LineSegment(sp[1], middle2, Segment::Positive);
    LineSegment* start = new LineSegment(sp[0], middle1, Segment::Positive);
    end->next(start);

    return new SegmentedShape(this, start);
}


int
Rectangle::getNumLines() const
{
    return 4;
}


bool
Rectangle::containsPoint(const Point2D& p) const
{
    CLEAN_IF_DIRTY(this);

    return r_.containsPoint(p);
}


} // namespace geom
