#ifndef _BOX_HPP_
#define _BOX_HPP_

#include "centeredAABB.hpp"
#include "matrix4x4.hpp"

#include "range2D.hpp"

namespace Physics::Shapes
{
    // Box doesn't have a location vector
    // since the translation is defined in the transform matrix
    struct Box
    {
        static constexpr unsigned int nbPoints = 8;

        // Corresponds to the scale of the Box.
        // The scale is also defined in the transform Matrix,
        // however, getting it from the matrix takes too many computations.
        CenteredAABB aabb;

        // Contains Translation, Rotation and Size/Scale of the cube.
        Core::Maths::Matrix4x4 transform = Core::Maths::Matrix4x4::identity(4);

        inline std::array<Core::Maths::Vec3, nbPoints> getPoints() const;
        inline std::array<Core::Maths::Vec3, nbPoints> getDiagonalVectors() const;
        static inline Range2D projectOnAxis(const Core::Maths::Vec3& axis, const std::array<Core::Maths::Vec3, nbPoints>& points);
    };

    inline std::ostream& operator<<(std::ostream& stream, const Box& box);
}

#include "box.inl"

#endif