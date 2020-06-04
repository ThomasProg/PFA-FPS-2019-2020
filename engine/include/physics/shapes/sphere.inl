#include "sphere.hpp"

inline Physics::Shapes::Sphere::Sphere(const Core::Maths::Vec3& _center, const float _radius)
    : center(_center), radius(_radius)
{

}

inline bool Physics::Shapes::Sphere::isInside(const Core::Maths::Vec3& v) const
{
    return asCenteredSphere.isInside(v - center);
}

inline std::ostream& Physics::Shapes::operator<<(std::ostream& stream, const Sphere& sphere)
{
    stream << "p : " << sphere.center << '\t' << "Radius : " << sphere.radius;
    return stream;
}