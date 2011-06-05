#ifndef ELLIPSE_H_
#define ELLIPSE_H_

#include "Shape.h"
#include "LineBasedShape.h"
#include "GenericEllipse.h"
#include "GenericArc.h"

namespace geom
{


class Ellipse : public GenericEllipse, public Shape
{

public:

    Ellipse();

    Ellipse(const Point2D& center, const Point2D& radius);

    ~Ellipse();

public:

    void performCleaning() const;

    void calculateBoundingBox(GenericRect& bb) const;

    void moveBy(const Point2D& delta);

    SegmentedShape* toSegmentedShape() const;

protected:

    bool isIntersectedByRectangle(const Rectangle* r,
                         SegmentPointVector& isecPoints,
                         int& isecCount) const;

    bool isIntersectedByTriangle(const Triangle* t,
                         SegmentPointVector& isecPoints,
                         int& isecCount) const;

    bool isIntersectedByEllipse(const Ellipse* e,
                         SegmentPointVector& isecPoints,
                         int& isecCount) const;

    //using GenericEllipse::isIntersectedBy;

private:

    bool isIntersectedByLineBasedShape(const LineBasedShape* s,
                                       SegmentPointVector& isecPoints,
                                       int& isecCount) const;

    SegmentPoint* makeSegmentPoint(const Point2D& p) const;

};


} // namespace geom

#endif // ELLIPSE_H_
