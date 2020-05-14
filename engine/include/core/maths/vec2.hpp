#ifndef _VEC2_HPP_
#define _VEC2_HPP_

#include <cmath>

#include <iostream>

namespace Core::Maths
{
    class Vec2
    {
    public:
        float x = 0, y = 0;

    public :

        Vec2()  = default;
        ~Vec2() = default;
        Vec2(const Vec2& vec);
        Vec2(float f);
        Vec2(float x, float y);

        void operator=(const Vec2&);
        bool operator==(const Vec2& vec) const;
        bool operator!=(const Vec2& vec) const;
        bool operator<(const Vec2& vec) const;
        bool operator<=(const Vec2& vec) const;
        bool operator>(const Vec2& vec) const;
        bool operator>=(const Vec2& vec) const;

        float& operator[](int id);
        float& operator[](std::string id);

        static Vec2 zero;
        static Vec2 up;
        static Vec2 down;
        static Vec2 left;
        static Vec2 right; 
        
        Vec2    add(const Vec2&) const;
        Vec2    operator+(const Vec2& vec) const;

        Vec2    substract(const Vec2& vec) const;
        Vec2    operator-(const Vec2& vec) const;

        Vec2    scale(float f) const;
        Vec2    operator*(const float f) const
        {
            return Vec2{x * f, y * f};
        }

        Vec2    divide(float f) const;
        Vec2    operator/(const float f) const;

        Vec2    operator+=(const Vec2& vec) const;
        Vec2&   operator+=(const Vec2& vec);
        Vec2    operator-=(const Vec2& vec) const;
        Vec2&   operator-=(const Vec2& vec);

        Vec2    operator*=(const float f) const;
        Vec2&   operator*=(const float f);
        Vec2    operator/=(const float f) const;
        Vec2&   operator/=(const float f);

        Vec2    operator++(int) const;
        Vec2&   operator++(int);
        Vec2    operator--(int) const;
        Vec2&   operator--(int);

        Vec2 normalize() const;
        Vec2& normalize();

        float dot_product(Vec2) const;
        float length() const;
        float cross_product(Vec2) const;
    };

    //input / output
    std::ostream& operator<<(std::ostream& stream, const Vec2& vector);
    std::istream& operator>>(std::istream& stream, Vec2& vector);

    //string
    std::string     operator+   (std::string& str, Vec2 vec);
    std::string&    operator+=  (std::string& str, Vec2 vec);

    //float
    Vec2 operator*  (float f, const Vec2& vec);
    Vec2 operator/  (float f, const Vec2& vec);

    //void
    Vec2 operator-  (const Vec2& vec);
    Vec2& operator- (Vec2& vec);

    float operator,(const Vec2&, const Vec2&);
}

#endif