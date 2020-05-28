#include "aabb.hpp"

#include "sphere.hpp"
#include "box.hpp"

void Physics::Shapes::AABB::setFrom(const Sphere& sphere)
{
    centeredAABB.size = {sphere.radius, sphere.radius, sphere.radius};
    location = sphere.center;
}

void Physics::Shapes::AABB::setFrom(const Box& box)
{
    location = box.transform.getTranslationVector();
    for (unsigned int axis = 0; axis < Core::Maths::Vec3::getAxisNumber(); ++axis)
    {
        centeredAABB.size[axis] = std::max(box.transform[axis][0], std::max(box.transform[axis][1], box.transform[axis][2]));
    }
}

Physics::Shapes::AABB Physics::Shapes::AABB::operator+(const Physics::Shapes::AABB& rhs) const
{
    AABB newAabb;
    
    const Core::Maths::Vec3 minLhs = location - centeredAABB.size;
    const Core::Maths::Vec3 minRhs = rhs.location - rhs.centeredAABB.size;
    Core::Maths::Vec3 min;
    for (unsigned int axis = 0; axis < Core::Maths::Vec3::getAxisNumber(); ++axis)
        min[axis] = minLhs[axis] < minRhs[axis] ? minLhs[axis] : minRhs[axis];

    const Core::Maths::Vec3 maxLhs = location + centeredAABB.size;
    const Core::Maths::Vec3 maxRhs = rhs.location + rhs.centeredAABB.size;
    Core::Maths::Vec3 max;
    for (unsigned int axis = 0; axis < Core::Maths::Vec3::getAxisNumber(); ++axis)
        max[axis] = maxLhs[axis] > maxRhs[axis] ? maxLhs[axis] : maxRhs[axis];

    newAabb.centeredAABB.size = (max - min) / 2.f;
    newAabb.location = min + newAabb.centeredAABB.size;

    return newAabb;
}
