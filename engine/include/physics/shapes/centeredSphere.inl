#include "centeredSphere.hpp"

inline bool Physics::Shapes::CenteredSphere::isInside(const Core::Maths::Vec3& v) const noexcept
{
    return v.vectorSquareLength() < radius * radius;
}

