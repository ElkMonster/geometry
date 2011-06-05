#include "GenericRect.h"

#include "Helpers.h"

namespace geom
{


GenericRect::GenericRect()
:   Dirtable(),
    p1_(Point2D(0,0)),
    p2_(Point2D(1,1))
{}


GenericRect::GenericRect(const Point2D& p1, const Point2D& p2)
:   Dirtable(),
    p1_(p1),
    p2_(p2)
{}


GenericRect::~GenericRect()
{}


void
GenericRect::performCleaning() const
{
    Point2D t1 = p1_;
    Point2D t2 = p2_;

    p1_ = min(t1, t2);
    p2_ = max(t1, t2);

    extents_ = p2_ - p1_;

    center_ = p1_ + extents_ / 2;

    bottomLeft_.x = p1_.x;
    bottomLeft_.y = p2_.y;

    topRight_.x = p2_.x;
    topRight_.y = p1_.y;

    lines_[0].p1p2(p1_, topRight_);
    lines_[1].p1p2(topRight_, p2_);
    lines_[2].p1p2(p2_, bottomLeft_);
    lines_[3].p1p2(bottomLeft_, p1_);
}


MAKE_SETTER(GenericRect::p1(const Point2D& p), p1_, p)

MAKE_SETTER(GenericRect::p2(const Point2D& p), p2_, p)

MAKE_GETTER(const Point2D& GenericRect::p1() const, p1_)

MAKE_GETTER(const Point2D& GenericRect::p2() const, p2_)

MAKE_GETTER(const Point2D& GenericRect::extents() const, extents_)

MAKE_GETTER(const GenericLine* GenericRect::lines() const, lines_)


void
GenericRect::moveBy(const Point2D& delta)
{
    p1_ += delta;
    p2_ += delta;
    dirty_ = true;
}


bool
GenericRect::isIntersectedByRect(const GenericRect& other) const
{
    CLEAN_IF_DIRTY(this);
    CLEAN_IF_DIRTY(&other);

    Point2D centerDist = (center_ - other.center_).abs();
    Point2D dmax = (extents_ + other.extents_) / 2;

    // Is some part of this within other's horizontal range, or vice versa?
    bool withinHoriz = centerDist.x <= dmax.x;

    // Is some part of this within other's vertical range, or vice versa?
    bool withinVert = centerDist.y <= dmax.y;

    return withinHoriz && withinVert;
//    if (withinHoriz || withinVert)
//    {
//        Point2D dmin = (extents_ - other.extents_).abs() / 2;
//        bool isIntersectedByHoriz = (centerDist.y >= dmin.y) && withinHoriz;
//        bool isIntersectedByVert = (centerDist.x >= dmin.x) && withinVert;
//
//        return isIntersectedByHoriz || isIntersectedByVert;
//    }
//
//    return false;
}


bool
GenericRect::isIntersectedByRect(
    const GenericRect& other, GenericRect& intersecRect) const
{
    bool bIntersects = false;

    if (isIntersectedByRect(other))
    {
        bIntersects = true;
        intersecRect = GenericRect(max(p1_, other.p1_), min(p2_, other.p2_));
    }

    return bIntersects;
}


bool
GenericRect::containsRect(const GenericRect& other) const
{
    CLEAN_IF_DIRTY(this);
    CLEAN_IF_DIRTY(&other);
    return (p1_ <= other.p1_) && (p2_ >= other.p2_);
}


bool
GenericRect::containsPoint(const Point2D& p) const
{
    CLEAN_IF_DIRTY(this);
    return ((p >= p1_) && (p <= p2_));
}


std::ostream&
operator<<(std::ostream& out, const GenericRect& rect)
{
    const Point2D& p1 = rect.p1_;
    const Point2D& p2 = rect.p2_;
    out << "GenericRect (" << p1.x << ", " << p1.y << ") -- (" << p2.x << ", "
        << p2.y << ")";
    return out;
}


} // namespace geom
