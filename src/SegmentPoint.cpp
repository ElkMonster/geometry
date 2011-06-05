#include "SegmentPoint.h"
//#include <iostream>

namespace geom
{


SegmentPoint::SegmentPoint(float x, float y, float t, const GenericShapeElement* parent)
:   Point2D(x, y),
    t(t),
    parent(parent),
    t2(0.f),
    parent2(0)
{}


SegmentPoint::SegmentPoint(const Point2D& p, float t, const GenericShapeElement* parent)
:   Point2D(p),
    t(t),
    parent(parent),
    t2(0.f),
    parent2(0)
{}


SegmentPoint::SegmentPoint(
    const Point2D& p,
    float t,
    const GenericShapeElement* parent,
    float t2,
    const GenericShapeElement* parent2)
:   Point2D(p),
    t(t),
    parent(parent),
    t2(t2),
    parent2(parent2)
{}


SegmentPoint::SegmentPoint(const SegmentPoint* other)
:   Point2D(*other),
    t(other->t),
    parent(other->parent),
    t2(other->t2),
    parent2(other->parent2)
{}


SegmentPoint::~SegmentPoint()
{
    //std::cout << *this << " Destructor" << std::endl;
}


} // namespace geom
