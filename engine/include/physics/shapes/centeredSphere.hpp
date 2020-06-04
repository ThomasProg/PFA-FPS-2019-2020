#ifndef _CENTERED_SPHERE_HPP_
#define _CENTERED_SPHERE_HPP_

#include "vec3.hpp"

namespace Physics::Shapes
{
    // origin centered sphere
    struct CenteredSphere
    {
        float radius;

        inline bool isInside(const Core::Maths::Vec3& v) const noexcept;
    };
}

#include "centeredSphere.inl"

#endif