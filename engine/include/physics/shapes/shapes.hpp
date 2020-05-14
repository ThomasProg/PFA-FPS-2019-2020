#ifndef _SHAPES_HPP_
#define _SHAPES_HPP_

#include "matrix4x4.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

#include <limits.h>

struct Segment 
{
    Core::Maths::Vec2 p1, p2;
};

struct Triangle
{
    Core::Maths::Vec2 p1, p2, p3;
};

struct Prism
{
    Triangle triangle;

    Core::Maths::Vec2 zRange;
};

#include "segment3D.hpp"
#include "rectangle.hpp"
#include "centeredSphere.hpp"
#include "sphere.hpp"
#include "cylinder.hpp"
#include "range2D.hpp"
#include "centeredAABB.hpp"
#include "aabb.hpp"
#include "box.hpp"
#include "segmentHit.hpp"

// inl

inline AABB::AABB(const Box& box) : centeredAABB(box.aabb), location(box.transform.getTranslationVector())
{

}

#endif