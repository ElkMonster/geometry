#include "SegmentPointVector.h"

#include <algorithm>

namespace geom
{


struct lessOp
{
    bool operator()(const SegmentPoint* p1, const SegmentPoint* p2) const
    {
        if (p1->parent == p2->parent)
        {
            return p1->t < p2->t;
        }
        else
        {
            return p1->parent < p2->parent;
        }
    }
};


void
SegmentPointVector::sort()
{
    std::sort(begin(), end(), lessOp());
}


bool
SegmentPointVector::hasPoint(const Point2D& p) const
{
    for (const_iterator i = begin(); i != end(); ++i)
    {
        if (**i == p)
        {
            return true;
        }
    }
    return false;
}


void
SegmentPointVector::deleteReferencedObjectsAndClear()
{
    for (const_iterator i = begin(); i != end(); ++i)
    {
        delete *i;
    }
    clear();
}


SegmentPointVector::RangeIterator::RangeIterator(
    const SegmentPointVector& v, const GenericShapeElement* commonParent)
:   v_(v),
    commonParent_(commonParent),
    idx_(0)
{
    // TODO brute force! -> search for first element in a more efficient way
    for (unsigned int i = 0; i != v_.size(); ++i)
    {
        if (v_[i]->parent == commonParent_)
        {
            idx_ = i;
            break;
        }
    }
}


bool
SegmentPointVector::RangeIterator::endReached() const
{
    return (idx_ >= v_.size()) || (v_[idx_]->parent != commonParent_);
}


void
SegmentPointVector::RangeIterator::operator++()
{
    ++idx_;
}


SegmentPoint*
SegmentPointVector::RangeIterator::operator*() const
{
    return v_[idx_];
}


} // namespace geom

