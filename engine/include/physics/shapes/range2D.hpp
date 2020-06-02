#ifndef _RANGE_2D_HPP_
#define _RANGE_2D_HPP_

#include <limits>

namespace Physics::Shapes
{
    struct Range2D 
    {
        float min, max;

        Range2D()
            : min(std::numeric_limits<float>::max()), max(std::numeric_limits<float>::min())
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