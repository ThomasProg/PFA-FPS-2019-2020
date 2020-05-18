#include "collisions.hpp"

inline bool Collisions::isNearlyNull(float f)
{
    return f >= negEpsilon && f <= epsilon;
} 

inline bool Collisions::areNearlyEqual(float f1, float f2)
{
    return abs(f1 - f2) <= epsilon;
}

inline bool Collisions::isProjectionBetween0AndV2SquareLength(const Core::Maths::Vec3& v1, const Core::Maths::Vec3& v2)
{
    float proj = Core::Maths::Vec3::dotProduct(v1, v2);
    return (proj >= 0 && proj <= v2.vectorSquareLength());
}

inline bool Collisions::areHigherThan(float p1, float p2, float f)
{
    return (p1 > f && p2 > f);
}

inline bool Collisions::areLessThan(float p1, float p2, float f)
{
    return (p1 < f && p2 < f);
}

template<class T>
constexpr const T& Collisions::clamp(const T& v, const T& lo, const T& hi)
{
    assert( !(hi < lo) );
    return (v < lo) ? lo : (hi < v) ? hi : v;
}