#ifndef _SPHERE_HPP_
#define _SPHERE_HPP_

#include "centeredSphere.hpp"

namespace Physics::Shapes
{
    struct Sphere
    {
        Core::Maths::Vec3 center;

        union 
        {
            float radius;
            CenteredSphere asCenteredSphere; 
        };

        inline Sphere() = default;
        inline Sphere(const Core::Maths::Vec3& _center, const float _radius);
        inline bool isInside(const Core::Maths::Vec3& v) const;
    };

    inline std::ostream& operator<<(std::ostream& stream, const Sphere& sphere);
}

#include "sphere.inl"

#endif