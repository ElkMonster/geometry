#ifndef SEGMENT_H_
#define SEGMENT_H_

#include "SegmentPoint.h"

namespace geom
{


class Segment
{

public:

    enum SegmentType { Positive, Negative };

    class Iterator
    {

    public:

        Iterator(Segment* start);

        bool endReached() const;

        void operator++();

        Segment* operator*() const;

    private:

        Segment* start_;

        Segment* current_;

        bool startPassed_;

    };

    Segment(const SegmentPoint& start, SegmentType type);

    Segment(const SegmentPoint& start, Segment* next, SegmentType type);

    virtual ~Segment();

public:

    virtual void makeClean() const;

    const SegmentPoint& start() const;

    const SegmentPoint& end() const;

    const GenericShapeElement* parent() const;

    void insertNewSegment(const SegmentPoint& p, SegmentType type);

    void insertSegment(Segment* s);

    Segment* next() const;

    void next(Segment* s);

    const SegmentType type;

protected:

    virtual Segment* duplicate(const SegmentPoint& start, SegmentType type) const = 0;

protected:

    SegmentPoint start_;

    Segment* next_;


};


} // namespace geom

#endif // SEGMENT_H_
