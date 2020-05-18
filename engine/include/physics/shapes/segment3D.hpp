#ifndef _SEGMENT_3D_HPP_
#define _SEGMENT_3D_HPP_

#include "vec3.hpp"

struct Segment3D
{
    Core::Maths::Vec3 p1, p2;

    inline float squaredLength() const noexcept
    {
        return (p1 - p2).vectorSquareLength();
    }
};

inline std::ostream& operator<<(std::ostream& stream, const Segment3D& seg)
{
    stream << "p1 : " << seg.p1 << '\t' << "p2 : " << seg.p2;
    return stream;
}

#endif