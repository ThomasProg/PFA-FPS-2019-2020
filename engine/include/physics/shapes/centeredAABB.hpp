#ifndef _CENTERED_ABBB_HPP_
#define _CENTERED_ABBB_HPP_

#include "vec3.hpp"

namespace Physics::Shapes
{
    struct CenteredAABB
    {
        CenteredAABB() = default;

        // Location is 0, 0, 0, since the aabb is centered on the origin
        Core::Maths::Vec3 size = {1, 1, 1};

        inline CenteredAABB(const Core::Maths::Vec3 v);
        inline CenteredAABB(float x, float y, float z);

        inline CenteredAABB(const CenteredAABB& aabb) = default;
        
        inline bool isPointInside(const Core::Maths::Vec3& v) const;
    };
}

#include "centeredAABB.inl"

#endif