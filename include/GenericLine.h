#ifndef LINE_H_
#define LINE_H_

#include <ostream>

#include "GenericShapeElement.h"
#include "Dirtable.h"
#include <Point2D.h>
#include "SegmentPointVector.h"


namespace geom
{


class GenericLine : public GenericShapeElement, public Dirtable
{

public:

    GenericLine();

    GenericLine(const Point2D& p1, const Point2D& p2);

    ~GenericLine();

public:

    void performCleaning() const;

    bool isIntersectedBy(const GenericLine& other,
                         SegmentPointVector& isecPoints,
                         int& isecCount) const;

    bool isIntersectedByLines(const GenericLine others[],
                              int numOthers,
                              SegmentPointVector& isecPoints,
                              int& isecCount) const;

    static bool areIntersectedBy(const GenericLine lines[],
                                 int numLines,
                                 const GenericLine otherLines[],
                                 int numOtherLines,
                                 SegmentPointVector& isecPoints,
                                 int& isecCount);

    bool containsPoint(const Point2D& p) const;

    bool containsPoint(const Point2D& p, float& t) const;

    const Point2D& p1() const;

    const Point2D& p2() const;

    void p1(const Point2D& p);

    void p2(const Point2D& p);

    void p1p2(const Point2D& p1, const Point2D& p2);

    const Point2D& vec() const;

    float m() const;

    float n() const;

    friend std::ostream& operator<<(std::ostream& out, const GenericLine& line);

private:

    bool intersectsLine(const GenericLine& other,
                        SegmentPointVector& isecPoints,
                        int& isecCount) const;

    bool isSuperposedBy(const GenericLine& other,
                        SegmentPointVector& intersecPoints,
                        int& count) const;

private:

    Point2D p1_;

    Point2D p2_;

    mutable Point2D vec_; // vec = p2 - p1

    mutable float m_;

    mutable float n_;

};


} // namespace geom

#endif // LINE_H_
