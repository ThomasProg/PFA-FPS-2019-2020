#include <iostream>
#include <cmath>
#include "vec2.hpp"

using namespace Core::Maths;

std::ostream& Core::Maths::operator<<(std::ostream& stream, const Core::Maths::Vec2& vector)
{
    stream << std::to_string(vector.x);
    stream << ",";
    stream << std::to_string(vector.y);
    return stream;
}

std::istream& Core::Maths::operator>>(std::istream& stream, Vec2& vector)
{
    float x, y;
    std::string xStr, yStr;
    char comma;

    stream >> xStr >> comma >> yStr;
    x = stof(xStr);
    y = stof(yStr);

    if (comma == ',')
    {
        vector.x = x;
        vector.y = y;
    }
    else 
    {
        vector.x = 0.f;
        vector.y = 0.f;
    }
    return stream;
}

/////////STRING///////////

std::string Core::Maths::operator+(std::string& str, Vec2 vec)
{
    return str + std::to_string(vec.x) + ',' + std::to_string(vec.y);
}

/////////FLOAT///////////

Vec2 Core::Maths::operator*(float f, const Vec2& vec)
{
    return vec.scale(f);
}

Vec2 Core::Maths::operator/(float f, const Vec2& vec)
{
    Vec2 returned (f / vec.x, f / vec.y);
    return returned;
}

std::string& Core::Maths::operator+=(std::string& str, Vec2 vec)
{
    str += std::to_string(vec.x) + ',' + std::to_string(vec.y);
    return str;
}

Vec2 Core::Maths::operator-(const Vec2& vec)
{
    return Vec2(- vec.x, - vec.y);
}

Vec2& Core::Maths::operator-(Vec2& vec)
{
    vec.x *= -1;
    vec.y *= -1;
    return vec;
}

float Core::Maths::operator,(const Vec2& a, const Vec2& b)
{
    return a.dot_product(b); 
}

////////Vec2/////////

Vec2 Vec2::zero     (0.f, 0.f);
Vec2 Vec2::up       (0.f, 1.f);
Vec2 Vec2::down     (0.f, -1.f);
Vec2 Vec2::left     (-1.f, 0.f);
Vec2 Vec2::right    (1.f, 0.f);

Vec2::Vec2(const Vec2& f32)
{
    this->x = f32.x;
    this->y = f32.y;
}

Vec2::Vec2(float f)
{
    this->x = f;
    this->y = f;
}

Vec2::Vec2(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Vec2::operator=(const Vec2& vec)
{
    this->x = vec.x;
    this->y = vec.y;
}

bool Vec2::operator==(const Vec2& vec) const
{
    return this->x == vec.x && this->y == vec.y;
}

bool Vec2::operator!=(const Vec2& vec) const
{
    return this->x != vec.x || this->y != vec.y;
}

bool Vec2::operator<(const Vec2& vec) const
{
    return this->length() < vec.length();
}

bool Vec2::operator<=(const Vec2& vec) const
{
    return this->length() <= vec.length();
}

bool Vec2::operator>(const Vec2& vec) const
{
    return this->length() > vec.length();
}

bool Vec2::operator>=(const Vec2& vec) const
{
    return this->length() >= vec.length();
}



float& Vec2::operator[](int id)
{
    if (id == 0 || id == 'x')
        return this->x;
    if (id == 1 || id == 'y')
        return this->y;
    else
        return this->x;
}

float& Vec2::operator[](std::string id)
{
    if (id == "x")
        return this->x;
    if (id == "y")
        return this->y;
    else
        return this->x;
}

///////SIMPLE OPERATIONS/////////

Vec2 Vec2::add(const Vec2& vec) const
{
    Vec2 newVec (this->x + vec.x, this->y + vec.y);
    return newVec;
}

Vec2 Vec2::operator+(const Vec2& vec) const
{
    return this->add(vec);
}

Vec2 Vec2::substract(const Vec2& vec) const
{
    Vec2 newVec (this->x - vec.x, this->y - vec.y);
    return newVec;
}

Vec2 Vec2::operator-(const Vec2& vec) const
{
    return this->substract(vec);
}

Vec2 Vec2::scale(float f) const
{
    Vec2 newVec (this->x * f, this->y * f);
    return newVec;
}

// Vec2 Core::Maths::Vec2::operator*(const float f) const
// {
//     return this->scale(f);
// }

Vec2 Vec2::divide(float f) const
{
    Vec2 newVec (this->x / f, this->y / f);
    return newVec;
}

Vec2 Vec2::operator/(const float f) const
{
    return this->divide(f);
}

Vec2 Vec2::operator+=(const Vec2& vec) const
{
    return this->add(vec);
}

Vec2& Vec2::operator+=(const Vec2& vec)
{
    x += vec.x;
    y += vec.y;
    return *this;
}

Vec2 Vec2::operator-=(const Vec2& vec) const
{
    return this->substract(vec);
}

Vec2& Vec2::operator-=(const Vec2& vec)
{
    x -= vec.x;
    y -= vec.y;
    return *this;
}

Vec2 Vec2::operator*=(const float f) const
{
    return this->scale(f);
}

Vec2& Vec2::operator*=(const float f)
{
    x *= f;
    y *= f;
    return *this;
}

Vec2 Vec2::operator/=(const float f) const
{
    return this->divide(f);
}

Vec2& Vec2::operator/=(const float f)
{
    x /= f;
    y /= f;
    return *this;
}

Vec2 Vec2::operator++(int) const
{
    Vec2 vec;
    float length = this->length();
    vec.x *= (length + 1);
    vec.y *= (length + 1) / length;
    return vec;
}

Vec2& Vec2::operator++(int)
{
    float length = this->length();
    this->x *= (length + 1.f) / length;
    this->y *= (length + 1.f) / length;
    return *this;
}

Vec2 Vec2::operator--(int) const
{
    Vec2 vec;
    float length = this->length();
    vec.x *= (length - 1);
    vec.y *= (length - 1) / length;
    return vec;
}

Vec2& Vec2::operator--(int)
{
    float length = this->length();
    this->x *= (length - 1.f) / length;
    this->y *= (length - 1.f) / length;
    return *this;
}

Vec2 Vec2::normalize() const
{
    float f = this->length();
    Vec2 newVec (this->y / f, this->x / f);
    return newVec;
}

Vec2& Vec2::normalize()
{
    float f = this->length();
    this->x = this->x / f;
    this->y = this->y / f;
    return *this;
}

///////COMPLEX OPERATIONS/////////

float Vec2::dot_product(Vec2 vec) const
{
    return this->x * vec.x + this->y * vec.y;
}

float Vec2::length() const
{
    return sqrt(pow(this->x, 2) + pow(this->y, 2));
}

float Vec2::cross_product(Vec2 vec) const
{
    return this->x * vec.y - this->y * vec.x;
}