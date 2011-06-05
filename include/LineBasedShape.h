#ifndef LINEBASEDSHAPE_H_
#define LINEBASEDSHAPE_H_

#include "Shape.h"
#include "GenericLine.h"

namespace geom
{


class LineBasedShape : public Shape
{

public:

    LineBasedShape(int numLines, ShapeType type);

    LineBasedShape(ShapeType type);

    virtual ~LineBasedShape();

public:

    const GenericLine* lines() const;

    virtual int getNumLines() const = 0;

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

private:

    bool isIntersectedByLineBasedShape(const LineBasedShape* s,
                                       SegmentPointVector& isecPoints,
                                       int& isecCount) const;

protected:

    GenericLine* lines_;

private:

    /// Determines whether lines_ is managed by this class an should thus be
    /// deleted on object destruction
    const bool doCleanupLines_;

};


} // namespace geom

#endif // LINEBASEDSHAPE_H_
