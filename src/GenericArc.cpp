#include "GenericArc.h"

#include <cmath>
#include "GenericEllipse.h"


namespace
{

    const double pi = std::atan(1.) * 4.;

}


namespace geom
{


GenericArc::GenericArc(GenericEllipse* ellipse, int qIdx)
:   ellipse_(ellipse),
    qIdx_(qIdx)
{}


bool
GenericArc::containsPoint(const Point2D& p, float& t) const
{
    if ((ellipse_->getQuadrant(p) == this) && ellipse_->isIntersectedByPoint(p))
    {
        t = getT(p);
        return true;
    }
    return false;
}


float
GenericArc::getT(const Point2D& p) const
{
    // Calculating t requires mapping point into Q0

    const Point2D& c = ellipse_->center();
    switch (qIdx_)
    {
    case 0:
    case 2:
        {
            // TODO really needed? should suffice to test against p == (0,0)
            if (p.x == c.x)
            {
                return 0.f;
            }

            Point2D d = p.abs() - c;
            float t = 1.0 - atan2(d.y, d.x) / (0.5 * pi);
            return t;
        }
    case 1:
    case 3:
        {
            // TODO really needed? should suffice to test against p == (0,0)
            if (p.y == c.y)
            {
                return 0.f;
            }

            // Swap x and y since in Q1 and Q3, t = 0 for y = 0 and t grows with
            // greater absolute values of y
            Point2D d(std::abs(p.y - c.x), std::abs(p.x - c.y));
            float t = 1.0 - atan2(d.y, d.x) / (0.5 * pi);
            return t;
        }
    default:
        return 0;
    }
}


} // namespace geom
