#include <cmath>

#include <cassert>

#include "vec4.hpp"
#include "vec3.hpp"

using namespace Core::Maths;

Vec4::Vec4(const Vec4& vec)
    : x(vec.x), 
      y(vec.y),
      z(vec.z),
      w(vec.w)
{
    
}

Vec4::Vec4(float x, float y, float z, float w)
    : x(x), 
      y(y),
      z(z),
      w(w)
{

}

Vec4::Vec4(const Vec3& vec3, float w)
{
    x = vec3.x;
    y = vec3.y;
    z = vec3.z;
    this->w = w;
}

float Vec4::GetMagnitude() const
{
    return pow(x, 2) + pow(y, 2) + pow(z, 2);
}

void Vec4::Normalize()
{
    float magnitude = GetMagnitude();
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}

void Vec4::Homogenize()
{
    if (w != 0)
    {
        x /= w;
        y /= w;
        z /= w;
        w /= w;
    }
}

Vec3 Vec4::getHomogenizedVec() const
{
    // LOGIC_ASSERT(w != 0, "Can't homogenize if w == 0");
    return Vec3(x / w, y / w, z / w);
}

float Vec4::DotProduct(const Vec4& lhs, const Vec4& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

float& Vec4::operator[](unsigned int index)
{
    // LOGIC_ASSERT(index < 4, "Invalid arguments : index is out of bounds (> 3).");
    switch (index)
    {
    case 0:
        return x;
        break;
    case 1:
        return y;
        break;
    
    case 2:
        return z;
        break;

    default: //the only left index is 3
        return w;
        break;
    }
}

float Vec4::operator[](unsigned int index) const
{
    // LOGIC_ASSERT(index < 4, "Invalid arguments : index is out of bounds (> 3).");
    switch (index)
    {
    case 0:
        return x;
        break;
    case 1:
        return y;
        break;
    
    case 2:
        return z;
        break;

    default: //the only left index is 3
        return w;
        break;
    }
}

Vec4 operator+(const Vec4& a, const Vec4& b)
{
    return Vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Vec4 Vec4::operator-(const Vec4& v) const
{
    return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vec4 operator*(const Vec4& vec, float f)
{
    return Vec4(vec.x * f, vec.y * f, vec.z * f, vec.w * f);
}

Vec4 operator*(float f, const Vec4& vec)
{
    return Vec4(vec.x * f, vec.y * f, vec.z * f, vec.w * f);
}

std::ostream& Core::Maths::operator<<(std::ostream& stream, const Vec4& vector)
{
    stream << "x : " << vector.x << " | y : " << vector.y << " | z : " << vector.z << " | w : " << vector.w << std::endl;
    return stream;
}

