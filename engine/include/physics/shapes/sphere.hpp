#ifndef _SPHERE_HPP_
#define _SPHERE_HPP_

#include "centeredSphere.hpp"

struct Sphere
{
    Core::Maths::Vec3 center;

    union 
    {
        float radius;
        CenteredSphere asCenteredSphere; 
    };

    Sphere() = default;

    Sphere(const Core::Maths::Vec3& _center, const float _radius)
        : center(_center), radius(_radius)
    {

    }

    inline bool isInside(const Core::Maths::Vec3& v) const
    {
        return asCenteredSphere.isInside(v - center);
    }
};

inline std::ostream& operator<<(std::ostream& stream, const Sphere& sphere)
{
    stream << "p : " << sphere.center << '\t' << "Radius : " << sphere.radius;
    return stream;
}

#endif