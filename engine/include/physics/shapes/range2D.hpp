#ifndef _RANGE_2D_HPP_
#define _RANGE_2D_HPP_

#include <limits.h>

namespace Physics::Shapes
{
    struct Range2D 
    {
        float min, max;

        Range2D()
            : min(INT_MAX), max(INT_MIN)
        {

        }

        Range2D(float rhs)
            : min(rhs), max(rhs)
        {

        }

        Range2D& operator=(float rhs)
        {
            min = max = rhs;

            return *this;
        }

        Range2D& operator+=(float rhs)
        {
            if (rhs < min)
                min = rhs;
            else if (rhs > max)
                max = rhs;

            return *this;
        }

        static bool isIntersecting(const Range2D& lhs, const Range2D& rhs)
        {
            return rhs.min < lhs.max && rhs.max > lhs.min;
        }
    };
}

#endif