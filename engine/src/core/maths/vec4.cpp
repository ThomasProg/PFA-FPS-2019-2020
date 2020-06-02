#include <cmath>

#include <cassert>

#include "vec4.hpp"
#include "vec3.hpp"

using namespace Core::Maths;

float Vec4::getMagnitude() const
{
    return pow(x, 2) + pow(y, 2) + pow(z, 2);
}

void Vec4::normalize()
{
    float magnitude = getMagnitude();
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}

void Vec4::homogenize()
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

float Vec4::dotProduct(const Vec4& lhs, const Vec4& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

float& Vec4::operator[](unsigned int index)
{
    return components[index];
    // // LOGIC_ASSERT(index < 4, "Invalid arguments : index is out of bounds (> 3).");
    // switch (index)
    // {
    // case 0:
    //     return x;
    //     break;
    // case 1:
    //     return y;
    //     break;
    
    // case 2:
    //     return z;
    //     break;

    // default: //the only left index is 3
    //     return w;
    //     break;
    // }
}

float Vec4::operator[](unsigned int index) const
{
    return components[index];
    // LOGIC_ASSERT(index < 4, "Invalid arguments : index is out of bounds (> 3).");
    // switch (index)
    // {
    // case 0:
    //     return x;
    //     break;
    // case 1:
    //     return y;
    //     break;
    
    // case 2:
    //     return z;
    //     break;

    // default: //the only left index is 3
    //     return w;
    //     break;
    // }
}

std::ostream& Core::Maths::operator<<(std::ostream& stream, const Vec4& vector)
{
    stream << "x : " << vector.x << " | y : " << vector.y << " | z : " << vector.z << " | w : " << vector.w << std::endl;
    return stream;
}
