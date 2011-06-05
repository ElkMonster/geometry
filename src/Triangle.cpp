#include "Triangle.h"

#include "Helpers.h"
#include "LineSegment.h"
#include "SegmentedShape.h"

namespace geom
{


Triangle::Triangle()
:   LineBasedShape(3, TTriangle),
    p1_(Point2D(0, 0)),
    p2_(Point2D(1, 0)),
    p3_(Point2D(0.5f, 1))
{}


Triangle::Triangle(const Point2D& p1, const Point2D& p2, const Point2D& p3)
:   LineBasedShape(3, TTriangle),
    p1_(p1),
    p2_(p2),
    p3_(p3)
{}


Triangle::~Triangle()
{}


void
Triangle::moveBy(const Point2D& delta)
{
    p1_ += delta;
    p2_ += delta;
    p3_ += delta;
    dirty_ = true;
}


void
Triangle::performCleaning() const
{
    lines_[0].p1(p1_);
    lines_[0].p2(p2_);

    lines_[1].p1(p2_);
    lines_[1].p2(p3_);

    lines_[2].p1(p3_);
    lines_[2].p2(p1_);
};


void
Triangle::p1p2p3(const Point2D& p1, const Point2D& p2, const Point2D& p3)
{
    p1_ = p1;
    p2_ = p2;
    p3_ = p3;
    dirty_ = true;
}


void
Triangle::calculateBoundingBox(GenericRect& bb) const
{
    bb.p1(min3(p1_, p2_, p3_));
    bb.p2(max3(p1_, p2_, p3_));
}


SegmentedShape*
Triangle::toSegmentedShape() const
{
    CLEAN_IF_DIRTY(this);

    SegmentPoint sp[] = {
        SegmentPoint(p1_, 0.f, &lines_[0]),
        SegmentPoint(p2_, 0.f, &lines_[1]),
        SegmentPoint(p3_, 0.f, &lines_[2])
    };

    LineSegment* end = new LineSegment(sp[2], Segment::Positive);
    LineSegment* middle = new LineSegment(sp[1], end, Segment::Positive);
    LineSegment* start = new LineSegment(sp[0], middle, Segment::Positive);
    end->next(start);

    return new SegmentedShape(this, start);
}


int
Triangle::getNumLines() const
{
    return 3;
}


} // namespace geom
