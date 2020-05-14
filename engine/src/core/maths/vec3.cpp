#include "vec3.hpp"

using namespace Core::Maths;


std::ostream& Core::Maths::operator<<(std::ostream& stream, const Vec3& rhs)
{
	stream << "x : " << rhs.x << "   " << "y : " << rhs.y << "   " << "z : " << rhs.z;
	return stream;
}

bool Vec3::isNearlyColinear(const Vec3& v1, const Vec3& v2, const decimal epsilon)
{
	return Vec3::crossProduct(v1, v2).vectorSquareLength() < epsilon;
} 

Core::Maths::Vec3& Core::Maths::Vec3::operator/=(float rhs)
{
	assert(rhs != 0.f && rhs != -0.f);

	x /= rhs;
	y /= rhs;
	z /= rhs;

	return *this;
}