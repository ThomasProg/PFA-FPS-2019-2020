#include "centeredAABB.hpp"

#define assertPositiveValues assert(size.x >= 0.f && size.y >= 0.f && size.z >= 0.f);

inline Physics::Shapes::CenteredAABB::CenteredAABB(const Core::Maths::Vec3 v) : size(v)
{
    assertPositiveValues
}

inline Physics::Shapes::CenteredAABB::CenteredAABB(float x, float y, float z) : size(x, y, z)
{
    assertPositiveValues
}

#undef assertPositiveValues

inline bool Physics::Shapes::CenteredAABB::isPointInside(const Core::Maths::Vec3& v) const
{
    return std::abs(v.x) < size.x
        && std::abs(v.y) < size.y
        && std::abs(v.z) < size.z;
}

