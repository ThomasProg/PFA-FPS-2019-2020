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
    centeredAABB.size = {0.f,0,0};
    location = box.transform.getTranslationVector();
    std::array<Core::Maths::Vec3, Box::nbPoints> diagonals = box.getDiagonalVectors();
    for (Core::Maths::Vec3& diagonal : diagonals)
    {
        for (unsigned int axis = 0; axis < Core::Maths::Vec3::getAxisNumber(); ++axis)
        {
            centeredAABB.size[axis] = std::max(centeredAABB.size[axis], std::abs(diagonal[axis]));
        }
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

std::ostream& Physics::Shapes::operator<<(std::ostream& stream, const Physics::Shapes::AABB& aabb)
{
    stream << "location : " << aabb.location << "\tsize  : " << aabb.centeredAABB.size << std::endl;
    return stream;
}