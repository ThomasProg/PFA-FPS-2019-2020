#ifndef _CYLINDER_HPP_
#define _CYLINDER_HPP_

#include "vec3.hpp"

namespace Physics::Shapes
{
    struct Cylinder
    {
        Core::Maths::Vec3 location;
        Core::Maths::Vec3 dirInWorldLoc;
        float radius;
    };

    inline std::ostream& operator<<(std::ostream& stream, const Cylinder& cylinder);
}

#include "cylinder.inl"

#endif