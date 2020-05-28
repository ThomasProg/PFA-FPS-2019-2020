#ifndef _AABB_HPP_
#define _AABB_HPP_

#include "centeredAABB.hpp"

namespace Physics::Shapes
{
    struct Box;

    struct Sphere;

    struct AABB
    {
        CenteredAABB centeredAABB;
        Core::Maths::Vec3 location;

        AABB() = default;
        AABB(const Box& box);

        void setFrom(const Sphere&);
        void setFrom(const Physics::Shapes::Box&);

        AABB operator+(const AABB&) const;
    };

    inline std::ostream& operator<<(std::ostream& stream, const AABB& aabb)
    {
        stream << "location : " << aabb.location << "\tsize  : " << aabb.centeredAABB.size << std::endl;
        return stream;
    }
}

#endif