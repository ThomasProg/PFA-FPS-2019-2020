#include "range2D.hpp"

inline Physics::Shapes::Range2D::Range2D()
    : min(std::numeric_limits<float>::max()), max(std::numeric_limits<float>::min())
{

}

inline Physics::Shapes::Range2D::Range2D(float rhs)
    : min(rhs), max(rhs)
{

}

inline Physics::Shapes::Range2D& Physics::Shapes::Range2D::operator=(float rhs)
{
    min = max = rhs;

    return *this;
}

inline Physics::Shapes::Range2D& Physics::Shapes::Range2D::operator+=(float rhs)
{
    if (rhs < min)
        min = rhs;
    else if (rhs > max)
        max = rhs;

    return *this;
}

inline bool Physics::Shapes::Range2D::isIntersecting(const Range2D& lhs, const Range2D& rhs)
{
    return rhs.min < lhs.max && rhs.max > lhs.min;
}

