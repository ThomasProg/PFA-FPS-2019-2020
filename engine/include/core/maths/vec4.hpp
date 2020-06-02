#ifndef _VEC4_HPP_
#define _VEC4_HPP_

#include <iostream>

namespace Core::Maths
{
    struct Vec3;
    class  Matrix4x4;

    struct Vec4
    {
    public:
        union 
        {
            struct 
            {
                float x, y, z, w;
            };

            float components[4];
        };

        inline constexpr Vec4(const Vec4& vec);
        inline constexpr Vec4(float x, float y, float z, float w);
        explicit inline constexpr Vec4(const Vec3& vec3, float w = 1.f);

        inline constexpr Vec4& operator=(const Vec4&) noexcept = default;

        inline constexpr Vec4 operator+(const Vec4&) const;
        inline constexpr Vec4 operator*(const Matrix4x4&) const = delete; //line convention
        inline constexpr Vec4 operator-(const Vec4&) const;
        float& operator[](unsigned int index);
        float operator[](unsigned int index) const;

        float getMagnitude() const;
        void  normalize();
        void  homogenize();
        Vec3  getHomogenizedVec() const;

        static float dotProduct(const Vec4&, const Vec4&);
    };

    inline constexpr Vec4 operator*(const Vec4&, float);
    inline constexpr Vec4 operator*(float, const Vec4&);
    std::ostream& operator<<(std::ostream& stream, const Vec4& vector);
}

#include "vec4.inl"

#endif