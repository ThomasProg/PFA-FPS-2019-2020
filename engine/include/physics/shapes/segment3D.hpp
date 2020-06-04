#ifndef _SEGMENT_3D_HPP_
#define _SEGMENT_3D_HPP_

#include "vec3.hpp"

namespace Physics::Shapes
{
    struct Segment3D
    {
        Core::Maths::Vec3 p1, p2;

        inline float squaredLength() const noexcept;
    };

    inline std::ostream& operator<<(std::ostream& stream, const Segment3D& seg);
}

#include "segment3D.inl"

#endif