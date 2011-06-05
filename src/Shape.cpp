#include "Shape.h"

#include "GeometryExceptions.h"

namespace geom
{

Shape::Shape(ShapeType type)
:   Dirtable(true),
    type_(type)
{}


Shape::~Shape()
{}


void
Shape::makeClean() const
{
    if (dirty_)
    {
        Dirtable::makeClean();
        calculateBoundingBox(bb_);
    }
}


const GenericRect&
Shape::bb() const
{
    CLEAN_IF_DIRTY(this);
    return bb_;
}


bool
Shape::isIntersectedBy(
    const Shape* s,
    SegmentPointVector& isecPoints,
    int& isecCount) const
{
    switch (s->type_)
    {
    case TRectangle:
        return isIntersectedByRectangle(
            reinterpret_cast<const Rectangle*>(s), isecPoints, isecCount);

    case TTriangle:
        return isIntersectedByTriangle(
            reinterpret_cast<const Triangle*>(s), isecPoints, isecCount);

    case TEllipse:
        return isIntersectedByEllipse(
            reinterpret_cast<const Ellipse*>(s), isecPoints, isecCount);

    default:
        // Shouldn't happen
        throw error::GeometryError(
            "Shape must be one of TRectangle, TTriangle, TEllipse");
    }
}


} // namespace geom
