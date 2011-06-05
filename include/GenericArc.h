#ifndef GENERICARC_H_
#define GENERICARC_H_

#include "GenericShapeElement.h"

namespace geom
{


// Forward declaration
class GenericEllipse;


class GenericArc : public GenericShapeElement
{

public:

    GenericArc(GenericEllipse* ellipse, int qIdx);

    bool containsPoint(const Point2D& p, float& t) const;

    float getT(const Point2D& p) const;


private:

    GenericEllipse* const ellipse_;

    int qIdx_;

};


} // namespace geom

#endif // GENERICARC_H_
