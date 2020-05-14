#ifndef _AABB_HPP_
#define _AABB_HPP_

#include "centeredAABB.hpp"

struct Box;

struct AABB
{
    CenteredAABB centeredAABB;
    Core::Maths::Vec3 location;

    AABB(const Box& box);
};

#endif