#include "segment3D.hpp"

inline float Physics::Shapes::Segment3D::squaredLength() const noexcept
{
    return (p1 - p2).vectorSquareLength();
}

inline std::ostream& Physics::Shapes::operator<<(std::ostream& stream, const Physics::Shapes::Segment3D& seg)
{
    stream << "p1 : " << seg.p1 << '\t' << "p2 : " << seg.p2;
    return stream;
}