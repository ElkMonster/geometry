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


bool
Triangle::containsPoint(const Point2D& p) const
{
    CLEAN_IF_DIRTY(this);

    if (bb().containsPoint(p))
    {
        // Solve eq.: p = p0 + s * v1 + t * v2; point is inside triangle if
        // s, t = [0, 1] and (s + t) = [0, 1].
        // To solve, make two equations:
        // (I) (p - p0) * v1 = (s * v1 + t * v2) * v1
        // (I) (p - p0) * v2 = (s * v1 + t * v2) * v2
        // which allows us to calculate the two unknowns s and t

        Point2D v0(p - p1_);
        Point2D v1(p2_ - p1_);
        Point2D v2(p3_ - p1_);

        float d02 = dot(v0, v2);
        float d01 = dot(v0, v1);
        float d11 = dot(v1, v1);
        float d12 = dot(v1, v2);
        float d22 = dot(v2, v2);

        float t = (d02 - d01 * d12 / d11) / (d22 - d12 * d12 / d11);
        float s = (d01 - t * d12) / d11;

        return between(s, 0.f, 1.f) && between(t, 0.f, 1.f) && ((s + t) <= 1.f);
    }

    return false;
}


} // namespace geom
