#ifndef _TRANSFORM_HPP_
#define _TRANSFORM_HPP_

#include "vec3.hpp"
#include "matrix4x4.hpp"

namespace Physics
{
    struct Transform
    {
        // rotation in radians
        Core::Maths::Vec3 location{0.f,0.f,0.f}, rotation{0.f,0.f,0.f}, scale {1, 1, 1};

        inline Core::Maths::Vec3 getForwardVector();
        inline Core::Maths::Vec3 getForwardXZVector();
        inline Core::Maths::Vec3 getRightXZVector();
    };
}

#include "transform.inl"

#endif