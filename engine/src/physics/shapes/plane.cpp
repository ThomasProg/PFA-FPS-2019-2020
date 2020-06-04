#include "plane.hpp"

Physics::Shapes::Plane::Plane(const Core::Maths::Vec3& normal, const float distance) noexcept
    : normal(normal), distance(distance)
{

}   

// with 3 points // valid
Physics::Shapes::Plane::Plane(const Core::Maths::Vec3& p1, const Core::Maths::Vec3& p2, const Core::Maths::Vec3& p3)
{
    this->normal   = Core::Maths::Vec3::crossProduct(p2 - p1, p3 - p1);
    // float dist = this->normal.vectorLength();
    this->normal  /= this->normal.vectorLength();
    this->distance = Core::Maths::Vec3::dotProduct(p2, normal);
}

Physics::Shapes::Plane::Plane(const Core::Maths::Vec3& p, const Core::Maths::Vec3& normal)
{
    this->normal   = normal;
    this->distance = Core::Maths::Vec3::dotProduct(p, normal);
}

float Physics::Shapes::Plane::getSignedDistance(const Core::Maths::Vec3& p) const noexcept
{
    return this->distance - Core::Maths::Vec3::dotProduct(p, this->normal);
}

float Physics::Shapes::Plane::getDistance(const Core::Maths::Vec3& p) const noexcept
{
    const float signedDist = getSignedDistance(p);
    return signedDist > 0 ? signedDist : - signedDist;
}

Core::Maths::Vec3 Physics::Shapes::Plane::getClosedPointOnPlane(const Core::Maths::Vec3& p) const noexcept
{
    return p + this->normal * getSignedDistance(p);
}

bool Physics::Shapes::Plane::getSide(const Core::Maths::Vec3& p) const noexcept
{
    return Core::Maths::Vec3::dotProduct(this->normal, p) > this->distance;
}

bool Physics::Shapes::Plane::isNearPlane(const Core::Maths::Vec3& p, const float epsilon) const noexcept
{
    return std::abs(Core::Maths::Vec3::dotProduct(this->normal, p) - this->distance) < epsilon;
}

// throws exception if there is no intersection
Core::Maths::Vec3 Physics::Shapes::Plane::get3PlanesIntersection(const Plane& plane1, const Plane& plane2, const Plane& plane3)
{
    Core::Maths::Matrix m1(3, 3);
    // Core::Maths::Matrix4x4 m1;
    m1[0][0] = plane1.normal.x;
    m1[0][1] = plane1.normal.y;
    m1[0][2] = plane1.normal.z;

    m1[1][0] = plane2.normal.x;
    m1[1][1] = plane2.normal.y;
    m1[1][2] = plane2.normal.z;

    m1[2][0] = plane3.normal.x;
    m1[2][1] = plane3.normal.y;
    m1[2][2] = plane3.normal.z;

    Core::Maths::Matrix result (1,3);// = Matrix::identity(4);
    // Matrix4x4 result;
    result[0][0] = plane1.distance;
    result[0][1] = plane2.distance;
    result[0][2] = plane3.distance;

    Core::Maths::Matrix e = m1.resolveEquation(result); // TODO : fix bug
    return Core::Maths::Vec3(e[0][0], e[0][1], e[0][2]);
}

