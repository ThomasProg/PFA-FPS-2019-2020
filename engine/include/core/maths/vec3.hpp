#ifndef _VEC3_HPP_
#define _VEC3_HPP_

#define decimal float

#include <iostream>
#include <array>
#include <sstream>

#include "assert.h"

namespace Core::Maths
{
	struct Vec4;
	struct Vec3;
	typedef Vec3 point3D;

	static float stringToFloat(const char* str)
	{
		if (str == nullptr)
			return 1.f;
		
		std::stringstream strValue;
		strValue << str;
		float f;
		strValue >> f;
		return f;
	}

	struct Vec3
	{
		union 
		{
			struct 
			{
				decimal x, y, z;
			};
			struct 
			{	
				std::array<decimal, 3> components;
			};
		};

		// used to verify if 2 vectors are equals
		static constexpr decimal epsilon = 0.1;

		constexpr inline Vec3() noexcept;
		constexpr inline Vec3(decimal x, decimal y, decimal z) noexcept;
		Vec3(const char* x, const char* y, const char* z) 
			: x(stringToFloat(x)), 
			y(stringToFloat(y)), 
			z(stringToFloat(z)) 
		{

		}
		Vec3(const Vec4& rhs);
		inline ~Vec3() noexcept = default;

		
		constexpr inline decimal vectorSquareLength() const noexcept;
		inline decimal vectorLength() const;
		inline Vec3 unitVector() const;
		inline Vec3 rotateAround(const Vec3& unitVec, decimal angle);

		constexpr inline void negate() noexcept { x = -x; y = -y; z = -z;}

		static bool isNearlyColinear(const Vec3& v1, const Vec3& v2, const decimal epsilon = __FLT_EPSILON__);

		
		static constexpr inline decimal dotProduct(const Vec3& u, const Vec3& v) noexcept;
		
		static constexpr inline Vec3 crossProduct(const Vec3& v1, const Vec3& v2) noexcept;
		
		// Inputs : coordinates of the triangle points
		static inline decimal getTriangleArea(const point3D& point1, 
											const point3D& point2, 
											const point3D& point3);
		
		// Inputs : 2 edges vectors
		static inline decimal getTriangleArea(const point3D& point2, 
											const point3D& point3);

		inline Vec3 operator-() const;

		inline Vec3 operator+(const Vec3&) const;
		inline Vec3 operator-(const Vec3&) const;
		inline Vec3 operator*(const Vec3&) const;
		inline Vec3 operator/(const Vec3&) const;
		Vec3 operator/(const float rhs) const
		{
			return Vec3(x / rhs, y / rhs, z / rhs);
		}

		inline Vec3& operator+=(const Vec3&);
		inline Vec3& operator-=(const Vec3&);
		inline Vec3& operator*=(const Vec3&);
		inline Vec3& operator/=(const Vec3&);

		inline bool operator==(const Vec3& rhs) const;
		inline bool operator<(const Vec3& rhs) const
		{
			return x < rhs.x && y < rhs.y && z < rhs.z;
		}
		inline bool operator>(const Vec3& rhs) const
		{
			return x > rhs.x && y > rhs.y && z > rhs.z;
		}

		Core::Maths::Vec3& operator/=(const float rhs);

		inline Vec3& operator*=(const float rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}

		inline Vec3& operator-=(const float rhs)
		{
			x -= rhs;
			y -= rhs;
			z -= rhs;
			return *this;
		}

		inline Vec3& operator+=(const float rhs)
		{
			x += rhs;
			y += rhs;
			z += rhs;
			return *this;
		}

		inline float operator[](unsigned int index) const
		{
			assert(index < 3);
			return components[index];
		}

		inline float& operator[](unsigned int index) 
		{
			assert(index < 3);
			return components[index];
		}


		inline static Vec3 getAxis(unsigned int index)
		{
			assert(index < 3);

			switch (index)
			{
			case 0:
				return Vec3{1, 0, 0};
			
			case 1:
				return Vec3{0, 1, 0};

			case 2:
				return Vec3{0, 0, 1};

			default:
				break;
			}

			// will never be executed
			return Core::Maths::Vec3{0.f, 0, 0};
		}

		constexpr inline static unsigned int getAxisNumber()
		{
			return 3;
		}
	};
	Core::Maths::Vec3 operator*(const Core::Maths::Vec3&, float);
	Core::Maths::Vec3 operator*(float, const Core::Maths::Vec3&);
	
	inline Core::Maths::Vec3 operator/(float, const Core::Maths::Vec3&);

	std::ostream& operator<<(std::ostream& stream, const Core::Maths::Vec3& rhs);
}

#include "vec4.hpp"

namespace Core::Maths
{
	inline
	Vec3::Vec3(const Vec4& rhs)
		: x(rhs.x), y(rhs.y), z(rhs.z)
	{}
}

#include "vec3.inl"

#endif