#ifndef _PLANE_HPP_
#define _PLANE_HPP_

#include "vec3.hpp"
#include "matrix.hpp"

#include <functional>

namespace Physics::Shapes
{
    class Plane
    {
    private:

    public:
        // n.x*x + n.y*y + n.z*z + d = 0
        Core::Maths::Vec3 normal;
        float distance;

        Plane(const Core::Maths::Vec3& normal, const float distance) noexcept;

        // with 3 points // valid
        Plane(const Core::Maths::Vec3& p1, const Core::Maths::Vec3& p2, const Core::Maths::Vec3& p3);

        Plane(const Core::Maths::Vec3& p, const Core::Maths::Vec3& normal);

        float getSignedDistance(const Core::Maths::Vec3& p) const noexcept;

        float getDistance(const Core::Maths::Vec3& p) const noexcept;

        Core::Maths::Vec3 getClosedPointOnPlane(const Core::Maths::Vec3& p) const noexcept;

        bool getSide(const Core::Maths::Vec3& p) const noexcept;

        bool isNearPlane(const Core::Maths::Vec3& p, const float epsilon = 0.1f) const noexcept;

        // throws exception if there is no intersection
        static Core::Maths::Vec3 get3PlanesIntersection(const Plane& plane1, const Plane& plane2, const Plane& plane3);
    };
}

#endif