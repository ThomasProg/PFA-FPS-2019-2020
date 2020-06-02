#include <cmath>
#include <cassert>
#include "vec3.hpp"

constexpr inline Core::Maths::Vec3::Vec3() noexcept
	: x(0.f), y(0.f), z(0.f)
{
	
}

constexpr inline Core::Maths::Vec3::Vec3(decimal x, decimal y, decimal z) noexcept
	: x(x), y(y), z(z)
{

}

constexpr inline decimal Core::Maths::Vec3::vectorSquareLength() const noexcept
{
	return (x * x + y * y + z * z);
}

inline
decimal Core::Maths::Vec3::vectorLength() const 
{
	return std::sqrt(vectorSquareLength());
}

inline
Core::Maths::Vec3 Core::Maths::Vec3::unitVector() const
{
	decimal d = vectorLength();
	if (d != 0.f)
		return { x / d, y / d , z / d};
	else
		return Core::Maths::Vec3{1.f, 0.f, 0.f};
}

constexpr inline decimal Core::Maths::Vec3::dotProduct(const Core::Maths::Vec3& u, const Core::Maths::Vec3& v) noexcept
{
	return { u.x * v.x 
           + u.y * v.y 
           + u.z * v.z};
}

constexpr inline Core::Maths::Vec3 Core::Maths::Vec3::crossProduct(const Core::Maths::Vec3& v1, const Core::Maths::Vec3& v2) noexcept
{
    return Core::Maths::Vec3
    (
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

inline
decimal Core::Maths::Vec3::getTriangleArea(const point3D& point1, 
                                  const point3D& point2, 
                                  const point3D& point3)
{
    return Core::Maths::Vec3::crossProduct(point2 - point1, point3 - point1).vectorLength() / 2;
}

inline
decimal Core::Maths::Vec3::getTriangleArea(const point3D& point2, 
                                  const point3D& point3)
{
    return Core::Maths::Vec3::crossProduct(point2, point3).vectorLength() / 2;
}

inline
Core::Maths::Vec3 Core::Maths::Vec3::rotateAround(const Core::Maths::Vec3& unitVec, decimal angle) 
{
    float c = cos(angle);

    return (*this) * c 
        + Core::Maths::Vec3::crossProduct(unitVec, (*this)) * sin(angle) 
        + unitVec * Core::Maths::Vec3::dotProduct(unitVec, (*this)) * (1 - c);
}

inline
Core::Maths::Vec3 Core::Maths::Vec3::operator-() const
{
    return Core::Maths::Vec3{-x, -y, -z};
}

inline
Core::Maths::Vec3 Core::Maths::Vec3::operator+(const Core::Maths::Vec3& rhs) const
{
    return Core::Maths::Vec3{x + rhs.x, y + rhs.y, z + rhs.z};
}

inline
Core::Maths::Vec3 Core::Maths::Vec3::operator-(const Core::Maths::Vec3& rhs) const
{
    return Core::Maths::Vec3{x - rhs.x, y - rhs.y, z - rhs.z};
}

inline
Core::Maths::Vec3 Core::Maths::Vec3::operator*(const Core::Maths::Vec3& rhs) const
{
    return Core::Maths::Vec3{x * rhs.x, y * rhs.y, z * rhs.z};
}

inline
Core::Maths::Vec3 Core::Maths::Vec3::operator/(const Core::Maths::Vec3& rhs) const
{
    return Core::Maths::Vec3{x / rhs.x, y / rhs.y, z / rhs.z};
}

inline
Core::Maths::Vec3& Core::Maths::Vec3::operator+=(const Core::Maths::Vec3& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

inline
Core::Maths::Vec3& Core::Maths::Vec3::operator-=(const Core::Maths::Vec3& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

inline
Core::Maths::Vec3& Core::Maths::Vec3::operator*=(const Core::Maths::Vec3& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}

inline
Core::Maths::Vec3& Core::Maths::Vec3::operator/=(const Core::Maths::Vec3& rhs)
{
    // MATHS_ASSERT(rhs.x != 0 && rhs.y != 0 && rhs.z != 0, "You can't divide by 0.");
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
}

inline
bool Core::Maths::Vec3::operator==(const Core::Maths::Vec3& rhs) const
{
    return crossProduct(*this, rhs).vectorSquareLength() < epsilon;
}

inline
Core::Maths::Vec3 Core::Maths::operator*(const Core::Maths::Vec3& vec, float f)
{
    return Core::Maths::Vec3{vec.x * f, vec.y * f, vec.z * f};
}


inline
Core::Maths::Vec3 Core::Maths::operator*(float f, const Core::Maths::Vec3& vec)
{
    return Core::Maths::Vec3{vec.x * f, vec.y * f, vec.z * f};
}

inline
Core::Maths::Vec3 Core::Maths::operator/(float f, const Core::Maths::Vec3& rhs)
{
    return Core::Maths::Vec3{f / rhs.x, f / rhs.y, f / rhs.z};
}


constexpr inline Core::Maths::Vec3 Core::Maths::Vec3::getAxis(unsigned int index) noexcept
{
    constexpr std::array<Vec3, 3> axes = 
    {
        Core::Maths::Vec3{ 1, 0, 0 },
        Core::Maths::Vec3{ 0, 1, 0 },
        Core::Maths::Vec3{ 0, 0, 1 }
    };

    assert(index < getAxisNumber());
    return axes[index];
}