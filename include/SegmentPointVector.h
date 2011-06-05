#ifndef SEGMENTPOINTVECTOR_H_
#define SEGMENTPOINTVECTOR_H_

#include "SegmentPoint.h"
#include "PointerAllocator.h"
#include <vector>
#include "GenericShapeElement.h"

namespace geom
{


// Forward declaration
//class GenericShapeElement;


//class SegmentPointVector : public std::vector<SegmentPoint*, PointerAllocator<SegmentPoint*> >
class SegmentPointVector : public std::vector<SegmentPoint*>
{

public:

    /// Needs SegmentPointVector::sort() to be called before construction!
    class RangeIterator
    {

    public:

        RangeIterator(
            const SegmentPointVector& v,
            const GenericShapeElement* commonParent);

        bool endReached() const;

        void operator++();

        SegmentPoint* operator*() const;

    private:

        const SegmentPointVector& v_;

        const GenericShapeElement* commonParent_;

        size_t idx_;

    };

    //SegmentPointVector();

    //~SegmentPointVector();

public:

    void sort();

    bool hasPoint(const Point2D& p) const;

    void deleteReferencedObjectsAndClear();


};


} // namespace geom


#endif // SEGMENTPOINTVECTOR_H_
