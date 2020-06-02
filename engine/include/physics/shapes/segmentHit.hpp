#ifndef _SEGMENT_HIT_HPP_
#define _SEGMENT_HIT_HPP_

#include "vec3.hpp"

namespace Physics::Shapes
{
    struct SegmentHit
    {
        float t;
        Core::Maths::Vec3 collisionPoint;
        Core::Maths::Vec3 normal;
    };
} 

#endif