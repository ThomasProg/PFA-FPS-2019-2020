#ifndef _RECTANGLE_HPP_
#define _RECTANGLE_HPP_

#include "vec3.hpp"

namespace Physics::Shapes
{
    struct Rectangle
    {
        Core::Maths::Vec3 point;
        Core::Maths::Vec3 seg1, seg2;
        Core::Maths::Vec3 normal;
    };
}

#endif