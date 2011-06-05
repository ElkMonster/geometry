#include "LineBasedShape.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "Ellipse.h"


namespace geom
{


LineBasedShape::LineBasedShape(int numLines, ShapeType type)
:   Shape(type),
    lines_(new GenericLine[numLines]),
    doCleanupLines_(true)
{}


LineBasedShape::LineBasedShape(ShapeType type)
:   Shape(type),
    lines_(0),
    doCleanupLines_(false)
{}


LineBasedShape::~LineBasedShape()
{
    if (doCleanupLines_)
    {
        delete[] lines_;
    }
}


bool
LineBasedShape::isIntersectedByRectangle(
    const Rectangle* r, SegmentPointVector& isecPoints, int& isecCount) const
{
    return isIntersectedByLineBasedShape(r, isecPoints, isecCount);
}


bool
LineBasedShape::isIntersectedByTriangle(
    const Triangle* t, SegmentPointVector& isecPoints, int& isecCount) const
{
    return isIntersectedByLineBasedShape(t, isecPoints, isecCount);
}


bool
LineBasedShape::isIntersectedByEllipse(
    const Ellipse* e, SegmentPointVector& isecPoints, int& isecCount) const
{
    if (!bb().isIntersectedByRect(e->bb()))
    {
        isecCount = 0;
        return false;
    }

    return e->intersects(lines_, getNumLines(), isecPoints, isecCount);
}


MAKE_GETTER(const GenericLine* LineBasedShape::lines() const, lines_)


bool
LineBasedShape::isIntersectedByLineBasedShape(
    const LineBasedShape* s,
    SegmentPointVector& isecPoints,
    int& isecCount) const
{
    if (!bb().isIntersectedByRect(s->bb()))
    {
        isecCount = 0;
        return false;
    }

    return GenericLine::areIntersectedBy(
        lines_, getNumLines(), s->lines(), s->getNumLines(), isecPoints, isecCount);
}


} // namespace geom
