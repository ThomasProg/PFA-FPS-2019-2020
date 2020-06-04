#include "box.hpp"

inline std::array<Core::Maths::Vec3, Physics::Shapes::Box::nbPoints> Physics::Shapes::Box::getPoints() const
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

inline std::array<Core::Maths::Vec3, Physics::Shapes::Box::nbPoints> Physics::Shapes::Box::getDiagonalVectors() const
{
    Core::Maths::Vec3 right   = transform.getXAxis() * aabb.size.x;
    Core::Maths::Vec3 up      = transform.getYAxis() * aabb.size.y;
    Core::Maths::Vec3 forward = transform.getZAxis() * aabb.size.z;

    Core::Maths::Vec3 center = transform.getTranslationVector();

    return { right + up + forward,
            right + up - forward,
            right - up + forward,
            right - up - forward,
            - right + up + forward,
            - right + up - forward,
            - right - up + forward,
            - right - up - forward };
}

inline Physics::Shapes::Range2D Physics::Shapes::Box::projectOnAxis(const Core::Maths::Vec3& axis, const std::array<Core::Maths::Vec3, nbPoints>& points)
{
    Range2D projection;
    for (const Core::Maths::Vec3& point : points)
    {
        projection += Core::Maths::Vec3::dotProduct(axis, point);
    }
    return projection;
}

inline std::ostream& Physics::Shapes::operator<<(std::ostream& stream, const Physics::Shapes::Box& box)
{
    stream << "aabb : " << box.aabb.size << '\n' << box.transform;
    return stream;
}
