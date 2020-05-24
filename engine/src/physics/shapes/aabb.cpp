#include "aabb.hpp"

#include "sphere.hpp"
#include "box.hpp"

void AABB::setFrom(const Sphere& sphere)
{
    centeredAABB.size = {sphere.radius, sphere.radius, sphere.radius};
    location = sphere.center;
}

void AABB::setFrom(const Box& box)
{
    location = box.transform.getTranslationVector();
    for (unsigned int axis = 0; axis < Core::Maths::Vec3::getAxisNumber(); ++axis)
    {
        centeredAABB.size[axis] = std::max(box.transform[axis][0], std::max(box.transform[axis][1], box.transform[axis][2]));
    }
}
