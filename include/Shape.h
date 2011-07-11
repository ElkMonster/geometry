#ifndef SHAPE_H_
#define SHAPE_H_

#include "Dirtable.h"
#include "GenericRect.h"
#include "SegmentPointVector.h"
#include "GenericShapeElement.h"


namespace geom
{

// Forward declarations
class Triangle;
class Rectangle;
class Ellipse;
class SegmentPoint;
class SegmentedShape;

class Shape : public Dirtable
{

public:

    enum ShapeType { TRectangle, TTriangle, TEllipse };

    Shape(ShapeType type);

    virtual ~Shape();

public:

    void makeClean() const;

    const GenericRect& bb() const;

    virtual void moveBy(const Point2D& delta) = 0;

    bool isIntersectedBy(const Shape* s,
                         SegmentPointVector& isecPoints,
                         int& isecCount) const;

    virtual SegmentedShape* toSegmentedShape() const = 0;

    virtual bool containsPoint(const Point2D& p) const = 0;

protected:

    virtual bool isIntersectedByRectangle(const Rectangle* r,
                                          SegmentPointVector& isecPoints,
                                          int& isecCount) const = 0;

    virtual bool isIntersectedByTriangle(const Triangle* t,
                                         SegmentPointVector& isecPoints,
                                         int& isecCount) const = 0;

    virtual bool isIntersectedByEllipse(const Ellipse* e,
                                        SegmentPointVector& isecPoints,
                                        int& isecCount) const = 0;

    virtual void calculateBoundingBox(GenericRect& bb) const = 0;

private:

    // Bounding box
    mutable GenericRect bb_;

    const ShapeType type_;

};

} // namespace geom

#endif // SHAPE_H_
