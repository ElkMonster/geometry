#include "SegmentedShape.h"


namespace geom
{


SegmentedShape::SegmentedShape(const Shape* shape, Segment* start)
:   shape_(shape),
    start_(start)
{}


SegmentedShape::~SegmentedShape()
{
    for (Segment::Iterator segmIt(start_); !segmIt.endReached(); ++segmIt)
    {
        delete *segmIt;
    }
}


void
SegmentedShape::addIntersections(SegmentPointVector& pv)
{
    if (start_ == 0)
    {
        return;
    }

    // RangeIterator requires sorted vector!
    pv.sort();

    for (Segment::Iterator segmIt(start_); !segmIt.endReached(); ++segmIt)
    {
        SegmentPointVector::RangeIterator pointIt(pv, (*segmIt)->parent());

        while (!pointIt.endReached())
        {
            // Make sure we're not on the last segment of the current Line or Arc
            if ((*pointIt)->parent == (*segmIt)->next()->parent())
            {
                // Check if current point-to-insert lies within current segment
                if ((*pointIt)->t < (*segmIt)->end().t)
                {
                    (*segmIt)->insertNewSegment(**pointIt, Segment::Negative);
                    ++pointIt;
                }
                else
                {
                    // Skip to next segment (which has the same parent)
                    ++segmIt;
                }
            }
            else
            {
                // Next segment has different parent, insert remaining points
                while (!pointIt.endReached())
                {
                    (*segmIt)->insertNewSegment(**pointIt, Segment::Negative);
                    ++pointIt;
                    ++segmIt;
                }
            }

        }
    }
}

std::ostream&
operator<<(std::ostream& out, const SegmentedShape& s)
{
    for (Segment::Iterator segmIt(s.start_); !segmIt.endReached(); ++segmIt)
    {
        const SegmentPoint& p = (*segmIt)->start();
        out << "[(" << p.x << ", " << p.y << ") "
            << ((*segmIt)->type == Segment::Positive ? "Pos" : "Neg" ) << "] ";
    }
    return out;
}


} // namespace geom
