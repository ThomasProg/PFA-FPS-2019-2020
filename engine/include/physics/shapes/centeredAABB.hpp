#ifndef _CENTERED_ABBB_HPP_
#define _CENTERED_ABBB_HPP_

#include "vec3.hpp"

namespace Physics::Shapes
{
    struct CenteredAABB
    {
        CenteredAABB() = default;

        // Location is 0, 0, 0, since the aabb is centered on the origin
        Core::Maths::Vec3 size;

        #define assertPositiveValues assert(size.x >= 0.f && size.y >= 0.f && size.z >= 0.f);

        inline CenteredAABB(const Core::Maths::Vec3 v) : size(v)
        {
            assertPositiveValues
        }

        inline CenteredAABB(float x, float y, float z) : size(x, y, z)
        {
            assertPositiveValues
        }

        #undef assertPositiveValues

        inline CenteredAABB(const CenteredAABB& aabb) = default;
        
        inline bool isPointInside(const Core::Maths::Vec3& v) const
        {
            return abs(v.x) < size.x
                && abs(v.y) < size.y
                && abs(v.z) < size.z;
        }
    };
}

#endif