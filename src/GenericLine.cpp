#include "GenericLine.h"

#include <cmath>
#include "Limits.h"
#include "Helpers.h"


namespace geom {


GenericLine::GenericLine()
:   Dirtable(true),
    p1_(Point2D(0,0)),
    p2_(Point2D(1,1))
{}


GenericLine::GenericLine(const Point2D& p1, const Point2D& p2)
:   Dirtable(true),
    p1_(p1),
    p2_(p2)
{}


GenericLine::~GenericLine()
{}


void
GenericLine::performCleaning() const
{
    vec_ = p2_ - p1_;
    m_ = (NEAR_ZERO(vec_.x)) ? 0.f : (vec_.y / vec_.x);
    n_ = m_ * (-p1_.x) + p1_.y;
}


MAKE_GETTER(const Point2D& GenericLine::p1() const, p1_)

MAKE_GETTER(const Point2D& GenericLine::p2() const, p2_)

MAKE_SETTER(GenericLine::p1(const Point2D& p), p1_, p)

MAKE_SETTER(GenericLine::p2(const Point2D& p), p2_, p)


void
GenericLine::p1p2(const Point2D& p1, const Point2D& p2)
{
    p1_ = p1;
    p2_ = p2;
    dirty_ = true;
}


MAKE_GETTER(const Point2D& GenericLine::vec() const, vec_)

MAKE_GETTER(float GenericLine::m() const, m_)

MAKE_GETTER(float GenericLine::n() const, n_)


bool
GenericLine::isIntersectedBy(
    const GenericLine& other,
    SegmentPointVector& isecPoints,
    int& isecCount) const
{
    isecCount = 0;

    CLEAN_IF_DIRTY(this);
    CLEAN_IF_DIRTY(&other);

    // Check for x = Const and p1 == p2 cases
    if (NEAR_ZERO(vec_.x))
    {
        if (NEAR_ZERO(vec_.y))
        {
            // p1 == p2, treat line as point
            float t;
            if (other.containsPoint(p1_, t))
            {
                isecPoints.push_back(new SegmentPoint(p1_, 0.f, this, t, &other));
                isecCount = 1;
            }
        }
        else // We have the x = Const case!
        {
            // Check if parallel (i.e., other line is also x = Const case)
            if (NEAR_ZERO(other.vec_.x))
            {
                // Check if collinear and segments superpose
                if (NEAR_EQUAL(p1_.x, other.p1_.x))
                {
                    isSuperposedBy(other, isecPoints, isecCount);
                }
            }
            else // Not parallel / collinear
            {
                Point2D p(p1_.x, other.m_ * p1_.x + other.n_);

                float t, t2;
                if (containsPoint(p, t) && other.containsPoint(p, t2))
                {
                    isecPoints.push_back(new SegmentPoint(p, t, this, t2, &other));
                    isecCount = 1;
                }
            }
        }

        return isecCount;
    }

    // Check if parallel and collinear
    if (NEAR_EQUAL(m_, other.m_) && NEAR_EQUAL(n_, other.n_))
    {
        // Check if segments superpose
        isSuperposedBy(other, isecPoints, isecCount);
    }
    else // Not parallel, thus intersection (somewhere)
    {
        Point2D p;
        // Check if other is x = Const case
        if (NEAR_ZERO(other.vec_.x))
        {
            p.x = other.p1_.x;
            p.y = m_ * p.x + n_;
        }
        else
        {
            p.x = (n_ - other.n_) / (other.m_ - m_);
            p.y = (other.m_ * n_ - m_ * other.n_) / (other.m_ - m_);
        }

        // Check for intersection of particular line segments
        float t, t2;
        if (containsPoint(p, t) && other.containsPoint(p, t2))
        {
            isecPoints.push_back(new SegmentPoint(p, t, this, t2, &other));
            isecCount = 1;
        }
    }

    return isecCount;
}


bool
GenericLine::isIntersectedByLines(
    const GenericLine others[],
    int numOthers,
    SegmentPointVector& isecPoints, int& isecCount) const
{
    isecCount = 0;

    // Check each of the other lines individually
    for (int i = 0; i < numOthers; ++i)
    {
        int n = 0;

        if (isIntersectedBy(others[i], isecPoints, n))
        {
            isecCount += n;
        }
    }

    return isecCount;
}


bool
GenericLine::areIntersectedBy(
    const GenericLine lines[],
    int numLines,
    const GenericLine otherLines[],
    int numOtherLines,
    SegmentPointVector& isecPoints,
    int& isecCount)
{
    isecCount  = 0;

    for (int i = 0; i < numLines; ++i)
    {
        int n = 0;
        if (lines[i].isIntersectedByLines(otherLines, numOtherLines, isecPoints, n))
        {
            isecCount += n;
        }
    }

    return isecCount;
}


bool
GenericLine::containsPoint(const Point2D& p) const
{
    float t;
    return containsPoint(p, t);
}


bool
GenericLine::containsPoint(const Point2D& p, float& t) const
{
    // (I)   p = p1 + t * (p2 - p1)
    // (II)  p.x = p1.x + t * (p2.x - p1.x)
    // (III) t = (p.x - p1.x) / (p2.x - p1.x)

    CLEAN_IF_DIRTY(this);

    // Check for x = Const and p1 == p2 cases
    if (NEAR_ZERO(vec_.x))
    {
        if (NEAR_ZERO(vec_.y))
        {
            // p1 == p2, treat line as point
            t = 0.f;
            return p1_ == p;
        }

        t = (p.y - p1_.y) / vec_.y;
    }
    else
    {
        t = (p.x - p1_.x) / vec_.x;
    }

    // (IV)  p.y = p1.y + t * (p2.y - p1.y)
    // The NEAR_EQUAL check is not necessary here and error prone due to
    // rounding errors (so remove that commented line some day?!)
    //return (between(t, 0.f, 1.f) && NEAR_EQUAL(p1_.y + t * vec_.y, p.y));
    return between(t, 0.f, 1.f);
}


bool
GenericLine::isSuperposedBy(
    const GenericLine& other, SegmentPointVector& isecPoints, int& isecCount) const
{
    // No need for CLEAN_IF_DIRTY here, as this method is available internally
    // only and cleaning should already have been performed in the calling
    // method

    isecCount = 0;

    // Factors that determine the t of the intersection point equation p in
    // p = p1 + t * (p2 - p1)
    float t[2]; // t of intersection points relative to this line
    float tOther[2]; // t of intersection points relative to other line

    SegmentPoint* isp[2] = {0, 0};

    bool containsOtherP1 = containsPoint(other.p1_, t[0]);
    bool containsOtherP2 = containsPoint(other.p2_, t[1]);

    if (containsOtherP1 && containsOtherP2)
    {
        // other is fully contained by this
        isecCount = 2;
        isp[0] = new SegmentPoint(other.p1_, t[0], this, 0.f, &other);
        isp[1] = new SegmentPoint(other.p2_, t[1], this, 1.f, &other);
    }
    else
    {
        bool otherContainsP1 = other.containsPoint(p1_, tOther[0]);
        bool otherContainsP2 = other.containsPoint(p2_, tOther[1]);

        if (otherContainsP1 && otherContainsP2)
        {
            // this is fully contained by other
            isecCount = 2;
            isp[0] = new SegmentPoint(p1_, 0.f, this, tOther[0], &other);
            isp[1] = new SegmentPoint(p2_, 1.f, this, tOther[1], &other);
        }
        else // Only possibilities left: partial or no superposition
        {
            if (containsOtherP1)
            {
                isp[0] = new SegmentPoint(other.p1_, t[0], this, 0.f, &other);
                isecCount++;
            }
            else if (containsOtherP2)
            {
                isp[0] = new SegmentPoint(other.p2_, t[1], this, 1.f, &other);
                isecCount++;
            }

            // Find second intersection point, check for end point intersection
            if (isecCount == 1)
            {
                if (otherContainsP1 && (*isp[0] != p1_))
                {
                    isp[1] = new SegmentPoint(p1_, 0.f, this, tOther[0], &other);
                    isecCount++;
                }
                else if (otherContainsP2 && (*isp[0] != p2_))
                {
                    isp[1] = new SegmentPoint(p2_, 1.f, this, tOther[1], &other);
                    isecCount++;
                }
                // ... else: Both lines share only one end point, count stays 1
            }
        }
    }

    if (isp[0])
    {
        isecPoints.push_back(isp[0]);
        if (isp[1])
        {
            isecPoints.push_back(isp[1]);
        }
    }

    return isecCount;
}


std::ostream&
operator<<(std::ostream& out, const GenericLine& line)
{
    const geom::Point2D& p1 = line.p1();
    const geom::Point2D& p2 = line.p2();
    out << "GenericLine (" << p1.x << ", " << p1.y << ") -- (" << p2.x << ", "
        << p2.y << ")";
    return out;
}


} // namespace geom
