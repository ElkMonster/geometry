#ifndef HELPERS_H_
#define HELPERS_H_

#include "Point2D.h"
#include "SegmentPoint.h"

namespace geom
{

inline bool between(float val, float min, float max)
{
    return val >= min && val <= max;
}


inline float min3(float v1, float v2, float v3)
{
    return std::min(v1, std::min(v2, v3));
}


inline float max3(float v1, float v2, float v3)
{
    return std::max(v1, std::max(v2, v3));
}


inline Point2D min3(const Point2D& p1, const Point2D& p2, const Point2D& p3)
{
    return min(p1, min(p2, p3));
}


inline Point2D max3(const Point2D& p1, const Point2D& p2, const Point2D& p3)
{
    return max(p1, max(p2, p3));
}


inline bool listContainsPoint(const SegmentPoint* const list[],
                       const Point2D& p,
                       int listSize)
{
    for (int i = 0; i < listSize; ++i)
    {
        if (list[i] == &p)
        {
            return true;
        }
    }
    return false;
}


} // namespace geom

#endif // HELPERS_H_
