#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "Shape.h"
#include "GenericLine.h"
#include "GenericRect.h"
#include "LineBasedShape.h"

namespace geom
{


class Rectangle : public LineBasedShape
{

public:

    Rectangle();

    Rectangle(const Point2D& p1, const Point2D& p2);

    ~Rectangle();

public:

    void p1p2(const Point2D& p1, const Point2D& p2);

    void performCleaning() const;

    void calculateBoundingBox(GenericRect& bb) const;

    void moveBy(const Point2D& delta);

    int getNumLines() const;

    SegmentedShape* toSegmentedShape() const;

    bool containsPoint(const Point2D& p) const;

private:

    GenericRect r_;

};


} // namespace geom

#endif // RECTANGLE_H_
