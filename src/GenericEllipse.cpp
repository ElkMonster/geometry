#include "GenericEllipse.h"

#include "Limits.h"
#include "Helpers.h"
//#include "Point2D.h"
#include "GenericRect.h"
#include "SegmentPoint.h"
#include "Math.h"
#include "GeometryExceptions.h"

#include <iostream>

namespace geom
{


GenericEllipse::GenericEllipse()
:   center_(Point2D(0,0)),
    radius_(Point2D(1,1)),
    quadrant_((GenericArc[]) {
        GenericArc(this, 0), GenericArc(this, 1), GenericArc(this, 2), GenericArc(this, 3) })
{}


GenericEllipse::GenericEllipse(const Point2D& center, const Point2D& radius)
:   center_(center),
    radius_(radius),
    quadrant_((GenericArc[]) {
        GenericArc(this, 0), GenericArc(this, 1), GenericArc(this, 2), GenericArc(this, 3) })
{}


GenericEllipse::~GenericEllipse()
{}


GenericEllipse&
GenericEllipse::moveBy(const Point2D& delta)
{
    center_ += delta;
    return *this;
}


const Point2D&
GenericEllipse::center() const
{
    return center_;
}


const Point2D&
GenericEllipse::radius() const
{
    return radius_;
}


bool
GenericEllipse::isIntersectedBy(
    const GenericEllipse* e,
    SegmentPointVector& isecPoints,
    int& isecCount) const
{
    isecCount = 0;

    // Circle (Ellipse 1 scaled on y axis):
    //     (I)  (x - m1.x)^2 + (y - m1.y)^2 = r^2
    //     (II) x^2 + y^2 = r^2  --> moved to origin
    // Ellipse 2:
    //     (I)  (x - m2.x)^2 / a^2 + (y - m2.y)^2 / b^2 = 1
    //     (II) (x - (m2.x - m1.x))^2 / a^2 + (y - (m2.y - m1.y))^2 / b^2 = 1
    //           --> moved by the same vector as circle/ellipse 1

    // A scale factor that transform the first ellipse into a circle in order to
    // simplify calculation.
    // For the calculation, make ellipse 1's vertical radius equal to the
    // horizontal one such that we have a circle, and scale the coordinate
    // system accordingly
    Point2D scale(1.f, radius_.x / radius_.y);
    Point2D m1 = center_ * scale;
    Point2D r1 = radius_ * scale;
    Point2D m2 = e->center_ * scale;
    Point2D r2 = e->radius_ * scale;

    geom::GenericRect bb1(m1 - r1, m1 + r1);
    geom::GenericRect bb2(m2 - r2, m2 + r2);

    // The rectangle enclosing the area in which the intersection point(s) is
    // (are) located
    GenericRect searchRect;
    if (!bb1.isIntersectedByRect(bb2, searchRect))
    {
        return false;
    }

    // Translate whole coordinate system so ellipse (circle) 1's center is at
    // origin to simplify our calculation; don't forget to re-translate later.
    // Ellipse 2 is now: (x - c)^2 / a^2 + (y - d)^2 / b^2 = 1
    double c = m2.x - m1.x;
    double d = m2.y - m1.y;

    double a_squ = r2.x * r2.x;
    double b_squ = r2.y * r2.y;
    double b_squ_div_a_squ = b_squ / a_squ;
    double r_squ = r1.x * r1.x;
    double d_squ = d*d;

    // P = b^2 / a^2 - 1
    double pp = b_squ_div_a_squ - 1.;
    // Q = -2 * b^2 / a^2 * c
    double qq = -2. * b_squ_div_a_squ * c;
    // R = r^2 + d^2 - b^2 + b^2 / a^2 *c^2
    double rr = r_squ + d_squ - b_squ + b_squ_div_a_squ * (c*c);

    double qq_squ = qq*qq;

    // Formula:
    // 0 =
    // (P^2)x^4 + (2PQ)x^3 + (Q^2 + 2PR + 4d^2)x^2 + (2QR)x + R^2 - 4(d^2)(r^2)

    // alpha = P^2
    double alpha = pp*pp;
    // beta = 2PQ
    double beta = 2. * pp * qq;
    // gamma = Q^2 + 2PR + 4d^2
    double gamma = qq_squ + (2. * pp * rr) + (4. * d_squ);
    // delta = 2QR
    double delta = 2. * qq * rr;
    // epsilon = R^2 - 4(d^2)(r^2)
    double epsilon = (rr*rr) - (4. * d_squ * r_squ);

    // Now solve alpha*x^4 + beta*x^3 + gamma*x^2 + delta*x + epsilon = 0
    double roots[4];
    int numRoots;
    quarticPolynomialRoots(alpha, beta, gamma, delta, epsilon, roots, numRoots);


    float fx[4], fy[4];
    for (int i = 0; i != numRoots; ++i)
    {
        // Rescale and retranslate y to normal coordinate system
        // y = sqrt(r^2 - x^2)
        fy[i] = (std::sqrt(r_squ - roots[i] * roots[i]) + m1.y) / scale.y;
        fx[i] = roots[i] + m1.x;
    }

    // Handle identical ellipses case
    if ((center_ == e->center_) && (radius_ == e->radius_))
    {
        numRoots = 1;
        fx[0] = center_.x;
        fy[0] = center_.y + radius_.y;
    }
    // Handle symmetric case where every x corresponds to two roots (x, +-y).
    // For some cases (two circles intersecting only, it seems), only one or two
    // roots will be found by solving the polynomial, so we have to add the
    // missing roots.
    else if ((center_.y == e->center_.y) && ((numRoots == 1) || (numRoots == 2)))
    {
        std::cout << "numroots "<< numRoots << std::endl;
        for (int i = 0; i != numRoots; ++i)
        {
            fx[i + numRoots] = fx[i];
            fy[i + numRoots] = 2. * center_.y - fy[i];
        }
        numRoots *= 2;
    }
    else
    {
        // Check if points are valid; for those that are not, we assume that
        // their y coordinate is on the wrong side of the x axis, so mirror it
        // along that axis. If the point is invalid in general,
        // makeSegmentPoint() will finally fail (see below).
        for (int i = 0; i != numRoots; ++i)
        {
            Point2D p(fx[i], fy[i]);
            if (!e->isIntersectedByPoint(p))
            {
                fy[i] = 2. * center_.y - fy[i];
            }
        }
    }

    for (int i = 0; i != numRoots; ++i)
    {
        Point2D p(fx[i], fy[i]);
        SegmentPoint* s = makeSegmentPoint(p, e->getQuadrant(p));
        isecPoints.push_back(s);
        ++isecCount;
    }

    return isecCount;
}


bool
GenericEllipse::isIntersectedBy(
    const GenericLine* lines,
    int numLines,
    SegmentPointVector& isecPoints,
    int& isecCount) const
{
    isecCount = 0;

    for (int i = 0; i < numLines; ++i)
    {
        int n = 0;
        if (isIntersectedByLine(lines[i], isecPoints, n))
        {
            isecCount += n;
        }
    }

    return isecCount;
}


bool
GenericEllipse::intersects(
    const GenericLine* lines,
    int numLines,
    SegmentPointVector& isecPoints,
    int& isecCount) const
{
    bool ret = isIntersectedBy(lines, numLines, isecPoints, isecCount);

    // Swap result's parents and t-factors since the relation is inverse here:
    // "this" (ellipse) *intersects* the lines, i.e., the function to call
    // should actually be "lines.areIntersectedBy(ellipse)". We don't have it,
    // though, in order to avoid unnecessary code doublettes.
    for (int i = 0; i != isecCount; ++i)
    {
        SegmentPoint* s = isecPoints[i];
        std::swap(s->parent, s->parent2);
        std::swap(s->t, s->t2);
    }
    return ret;
}


bool
GenericEllipse::isIntersectedByLine(
    const GenericLine& line,
    SegmentPointVector& isecPoints,
    int& isecCount) const
{
//    CLEAN_IF_DIRTY(this);
    isecCount = 0;

    // Ellipse:
    //     (x - c)^2 / a^2 + (y - d)^2 / b^2 = 1
    // Line:
    //     (x,y) = p1 + v * (p2 - p1)

    // Solution vars
    int nSolutions = 0;
    float x[2], y[2];

    const Point2D& p1 = line.p1();
    const Point2D& p2 = line.p2();

    float a_squ = radius_.x * radius_.x;

    // Handle vertical lines separately
    if (NEAR_ZERO(line.vec().x))
    {
        // Solve y1,2 = d + b * sqrt(1 - (x - c)^2 / a^2)
        // rr = expression in sqrt() -> (rr < 0) => no solution
        float rr = 1 - ((p1.x - center_.x) * (p1.x - center_.x) / a_squ);

        if (rr >= 0.f)
        {
            if (NEAR_ZERO(rr))
            {
                // One solution
                nSolutions = 1;
                y[0] = center_.y;
            }
            else
            {
                // Two solutions
                nSolutions = 2;
                float addend = sqrt(rr) * radius_.y;
                y[0] = center_.y + addend;
                y[1] = center_.y - addend;
            }

            for (int i = 0; i != nSolutions; ++i)
            {
                float f = (y[i] - p1.y) / (p2.y - p1.y);
                if (between(f, 0.f, 1.f))
                {
                    Point2D p(p1.x, y[i]);
                    isecPoints.push_back(makeSegmentPoint(p, &line));
                    ++isecCount;
                }
            }
        }

        return isecCount;
    }

    // Handle all non-vertical lines from here on

    // Line: y = m * x + n
    float m = line.m();
    float n = line.n();

    float a_squ_div_b_squ = a_squ / (radius_.y * radius_.y);

    float q = n - center_.y;
    float k = a_squ_div_b_squ * m * q - center_.x;
    float l = a_squ_div_b_squ * m * m + 1;
    float r = a_squ_div_b_squ * q * q + (center_.x * center_.x) - a_squ;

    // Solve x1,2 = - K / L +- sqrt((K / L)^2 - R / L)
    // rr = expression in sqrt() -> rr < 0 => no solution
    float k_div_l = k / l;
    float rr = k_div_l * k_div_l - (r / l);

    if (rr >= 0.f)
    {
        if (NEAR_ZERO(rr))
        {
            // One solution: x = - K / L +- 0
            nSolutions = 1;
            x[0] = -k_div_l;
        }
        else
        {
            // Two solutions
            nSolutions = 2;
            float rr_sqrt = sqrt(rr);
            x[0] = -k_div_l + rr_sqrt;
            x[1] = -k_div_l - rr_sqrt;
        };

        // Make sure intersection is between the line's end points via:
        // p1 + f * (p2 - p1) = (x,y): (0 <= f <= 1) ==> (x,y) between points
        for (int i = 0; i != nSolutions; ++i)
        {
            float f = (x[i] - p1.x) / (p2.x - p1.x);
            if (between(f, 0.f, 1.f))
            {
                Point2D p(x[i], m * x[i] + n);
                isecPoints.push_back(makeSegmentPoint(p, &line));
                ++isecCount;
            }
        }
    }

    return isecCount;
}


SegmentPoint*
GenericEllipse::makeSegmentPoint(
    const Point2D& p,
    const GenericShapeElement* parent2) const
{
    float t2;

    if (parent2->containsPoint(p, t2))
    {
        const GenericArc* parent = getQuadrant(p);
        return new SegmentPoint(p, parent->getT(p), parent, t2, parent2);
    }
    else
    {
        throw error::ConsistencyError(
            "GenericEllipse::makeSegmentPoint: Point is not part of parent2");
    }
}


const GenericArc*
GenericEllipse::getQuadrant(const Point2D& p) const
{
    //            | +y
    //            |
    //        Q3  |  Q0
    // -x ________|_________ +x
    //            |
    //        Q2  |  Q1
    //            |
    //            | -y

    int quadrant;

    if (p.x == center_.x)
    {
        // Special case p = (0,0): assign to Q0
        quadrant = (p.y >= center_.y) ? 0 : 2;
    }
    else if (p.y == center_.y)
    {
        quadrant = (p.x < center_.x) ? 3 : 1;
    }
    else
    {
        if (p.x < center_.x)
        {
            quadrant = (p.y < center_.y) ? 2 : 3;
        }
        else
        {
            quadrant = (p.y < center_.y) ? 1 : 0;
        }
    }

    return &quadrant_[quadrant];
}


bool
GenericEllipse::isIntersectedByPoint(const Point2D& p) const
{
    Point2D p0 = (p - center_);
    p0 = (p0 * p0) / (radius_ * radius_);
    return NEAR_EQUAL(p0.x + p0.y, 1.f);
}

/*void
GenericEllipse::identifyIsecPoints(
    const Point2D& first,
    const GenericEllipse* e,
    SegmentPointVector& isecPoints,
    int& isecCount) const
{

}*/


} // namespace geom
