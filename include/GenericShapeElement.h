#ifndef GENERICSHAPEELEMENT_H_
#define GENERICSHAPEELEMENT_H_

#include "Point2D.h"

namespace geom
{


// A common base class for GenericLine and GenericArc
class GenericShapeElement {

public:

    virtual bool containsPoint(const Point2D& p, float& t) const = 0;

};


} // namespace geom

#endif // GENERICSHAPEELEMENT_H_
