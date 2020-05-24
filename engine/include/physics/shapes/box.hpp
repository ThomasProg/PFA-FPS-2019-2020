#ifndef _BOX_HPP_
#define _BOX_HPP_

#include "centeredAABB.hpp"
#include "matrix4x4.hpp"

#include "range2D.hpp"

// Box doesn't have a location vector
// since the translation is defined in the transform matrix
struct Box
{
    Box() 
    {
        aabb.size = {1,1,1};
    }

    static constexpr unsigned int nbPoints = 8;

    // Corresponds to the scale of the Box.
    // The scale is also defined in the transform Matrix,
    // however, getting it from the matrix takes too many computations.
    CenteredAABB aabb;

    // Contains Translation, Rotation and Size/Scale of the cube.
    Core::Maths::Matrix4x4 transform;

    std::array<Core::Maths::Vec3, nbPoints> getPoints() const
    {
        Core::Maths::Vec3 right   = transform.getXAxis() * aabb.size.x;
        Core::Maths::Vec3 up      = transform.getYAxis() * aabb.size.y;
        Core::Maths::Vec3 forward = transform.getZAxis() * aabb.size.z;

        Core::Maths::Vec3 center = transform.getTranslationVector();

        return { center + right + up + forward,
                 center + right + up - forward,
                 center + right - up + forward,
                 center + right - up - forward,
                 center - right + up + forward,
                 center - right + up - forward,
                 center - right - up + forward,
                 center - right - up - forward };
    }

    static inline Range2D projectOnAxis(const Core::Maths::Vec3& axis, const std::array<Core::Maths::Vec3, nbPoints>& points)
    {
        Range2D projection;
        for (const Core::Maths::Vec3& point : points)
        {
            projection += Core::Maths::Vec3::dotProduct(axis, point);
        }
        return projection;
    }
};

inline std::ostream& operator<<(std::ostream& stream, const Box& box)
{
    stream << "aabb : " << box.aabb.size << '\n' << box.transform;
    return stream;
}

#endif