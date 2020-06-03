#include "vec4.hpp"

#include "vec3.hpp"

inline constexpr Core::Maths::Vec4::Vec4(const Vec4& vec)
    : x(vec.x), 
      y(vec.y),
      z(vec.z),
      w(vec.w)
{
    
}

inline constexpr Core::Maths::Vec4::Vec4(float x, float y, float z, float w)
    : x(x), 
      y(y),
      z(z),
      w(w)
{

}

inline constexpr Core::Maths::Vec4::Vec4(const Vec3& rhs, float w)
    : x(rhs.x), y(rhs.y), z(rhs.z), w(w)
{

}

inline constexpr Core::Maths::Vec4 Core::Maths::Vec4::operator-(const Core::Maths::Vec4& v) const
{
    return Core::Maths::Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

inline constexpr Core::Maths::Vec4 Core::Maths::Vec4::operator+(const Core::Maths::Vec4& rhs) const
{
    return Core::Maths::Vec4{rhs.x + x, rhs.y + y, rhs.z + z, rhs.w + w};
}

inline constexpr Core::Maths::Vec4 Core::Maths::operator*(const Core::Maths::Vec4& vec, float f)
{
    return Core::Maths::Vec4(vec.x * f, vec.y * f, vec.z * f, vec.w * f);
}

inline constexpr Core::Maths::Vec4 Core::Maths::operator*(float f, const Core::Maths::Vec4& vec)
{
    return Core::Maths::Vec4(vec.x * f, vec.y * f, vec.z * f, vec.w * f);
}

inline float& Core::Maths::Vec4::operator[](unsigned int index) noexcept
{
    return components[index];
}

inline float Core::Maths::Vec4::operator[](unsigned int index) const noexcept
{
    return components[index];
}