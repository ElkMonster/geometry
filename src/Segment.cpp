#include "Segment.h"

namespace geom
{


Segment::Segment(const SegmentPoint& start, SegmentType type)
:   type(type),
    start_(start),
    next_(0)
{}


Segment::Segment(const SegmentPoint& start, Segment* next, SegmentType type)
:   type(type),
    start_(start),
    next_(next)
{}


Segment::~Segment()
{}


void
Segment::makeClean() const
{
}


const SegmentPoint&
Segment::start() const
{
    return start_;
}


const SegmentPoint&
Segment::end() const
{
    return next_->start();
}


const GenericShapeElement*
Segment::parent() const
{
    return start_.parent;
}


void
Segment::insertNewSegment(const SegmentPoint& p, SegmentType type)
{
    insertSegment(duplicate(p, type));
}


void
Segment::insertSegment(Segment* s)
{
    if (s != 0)
    {
        s->next(next_);
        next_ = s;
    }
}


Segment*
Segment::next() const
{
    return next_;
}


void
Segment::next(Segment* s)
{
    next_ = s;
}


Segment::Iterator::Iterator(Segment* start)
:   start_(start),
    current_(start),
    startPassed_(false)
{}


bool
Segment::Iterator::endReached() const
{
    return (current_ != 0) && startPassed_ && (current_ == start_);
}


void
Segment::Iterator::operator++()
{
    current_ = current_->next();
    startPassed_ = true;
}


Segment*
Segment::Iterator::operator*() const
{
    return current_;
}


} // namespace geom
