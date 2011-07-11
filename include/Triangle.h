#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "LineBasedShape.h"
#include "Rectangle.h"
#include "GenericLine.h"

namespace geom
{

class Triangle : public LineBasedShape
{

public:

    Triangle();

    Triangle(const Point2D& p1, const Point2D& p2, const Point2D& p3);

    ~Triangle();

public:

    void moveBy(const Point2D& delta);

    void performCleaning() const;

    void p1p2p3(const Point2D& p1, const Point2D& p2, const Point2D& p3);

    const GenericLine* lines() const;

    SegmentedShape* toSegmentedShape() const;

    int getNumLines() const;

    bool containsPoint(const Point2D& p) const;

protected:

    void calculateBoundingBox(GenericRect& bb) const;

private:

    Point2D p1_;
    Point2D p2_;
    Point2D p3_;

};

} // namespace geom

#endif // TRIANGLE_H_
