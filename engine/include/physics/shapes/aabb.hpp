#ifndef _AABB_HPP_
#define _AABB_HPP_

#include "centeredAABB.hpp"

struct Box;

struct Sphere;

struct AABB
{
    CenteredAABB centeredAABB;
    Core::Maths::Vec3 location;

    AABB() = default;
    AABB(const Box& box);

    void setFrom(const Sphere&);
    void setFrom(const Box&);
};

#endif