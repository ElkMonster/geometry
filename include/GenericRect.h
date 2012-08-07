#ifndef GENERICRECT_H_
#define GENERICRECT_H_

#include <Point2D.h>
#include "SegmentPoint.h"
#include "GenericLine.h"
#include "Dirtable.h"

namespace geom
{


class GenericRect : public Dirtable
{

public:

    friend class Rectangle;

    GenericRect();

    GenericRect(const Point2D& p1, const Point2D& p2);

    virtual ~GenericRect();

public:

    void performCleaning() const;

    void p1(const Point2D& p);

    void p2(const Point2D& p);

    /** Note that a call to p1() will update the rectangle - if p2 hasn't been
     *  set, this will lead to seemingly false results. */
    const Point2D& p1() const;

    const Point2D& p2() const;

    const Point2D& extents() const;

    const GenericLine* lines() const;

    void moveBy(const Point2D& delta);

    bool isIntersectedByRect(const GenericRect& other) const;

    bool isIntersectedByRect(const GenericRect& other,
                             GenericRect& intersecRect) const;

    bool containsRect(const GenericRect& other) const;

    bool containsPoint(const Point2D& p) const;

    friend std::ostream& operator<<(std::ostream& out, const GenericRect& rect);

protected:

    mutable Point2D p1_;

    mutable Point2D p2_;

    mutable Point2D extents_;

    mutable Point2D bottomLeft_;

    mutable Point2D topRight_;

    mutable Point2D center_;

    mutable GenericLine lines_[4];

};

} // namespace geom

#endif // GENERICRECT_H_
