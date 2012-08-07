#ifndef GENERICELLIPSE_H_
#define GENERICELLIPSE_H_

#include <Point2D.h>
#include "SegmentPointVector.h"
#include "GenericLine.h"
#include "GenericArc.h"

namespace geom
{


class GenericEllipse : public Dirtable
{

public:

    GenericEllipse();

    GenericEllipse(const Point2D& center, const Point2D& radius);

    ~GenericEllipse();

public:

    //void performCleaning() const;

    GenericEllipse& moveBy(const Point2D& delta);

    bool isIntersectedBy(const GenericEllipse* e,
                         SegmentPointVector& isecPoints,
                         int& isecCount) const;

    bool isIntersectedBy(const GenericLine* lines,
                         int numLines,
                         SegmentPointVector& isecPoints,
                         int& isecCount) const;

    bool intersects(const GenericLine* lines,
                    int numLines,
                    SegmentPointVector& isecPoints,
                    int& isecCount) const;

    const Point2D& center() const;

    void center(const Point2D& center);

    const Point2D& radius() const;

    void radius(const Point2D& radius);

    const GenericArc* getQuadrant(const Point2D& p) const;

    bool isIntersectedByPoint(const Point2D& p) const;

protected:

    bool isIntersectedByLine(const GenericLine& line,
                             SegmentPointVector& isecPoints,
                             int& isecCount) const;

    SegmentPoint* makeSegmentPoint(const Point2D& p,
                                   const GenericShapeElement* parent2) const;

private:

    void identifyIsecPoints(const Point2D& first,
                            const GenericEllipse* e,
                            SegmentPointVector& isecPoints,
                            int& isecCount) const;

protected:

    Point2D center_;

    Point2D radius_;

    GenericArc quadrant_[4];

};


} // namespace geom

#endif // GENERICELLIPSE_H_
