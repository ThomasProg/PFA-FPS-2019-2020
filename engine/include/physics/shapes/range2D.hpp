#ifndef _RANGE_2D_HPP_
#define _RANGE_2D_HPP_

#include <limits>

namespace Physics::Shapes
{
    struct Range2D 
    {
        float min, max;

        inline Range2D();
        inline Range2D(float rhs);

        inline Range2D& operator=(float rhs);

        inline Range2D& operator+=(float rhs);

        static inline bool isIntersecting(const Range2D& lhs, const Range2D& rhs);
    };
}

#include "range2D.inl"

#endif