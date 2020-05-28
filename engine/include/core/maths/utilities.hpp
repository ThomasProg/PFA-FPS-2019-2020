#ifndef _MATHS_UTILITIES_HPP_
#define _MATHS_UTILITIES_HPP_

template<class T>
constexpr const T& clamp(const T& v, const T& low, const T& high)
{
    assert(!(high < low) && "Low value is hight than High value");
    return (v < low) ? low : (high < v) ? high : v;
}

template<class T, class U>
constexpr const T& lerp(const T& start, const T& end, const U& alpha)
{
    return start + (end - start) * alpha;
}

#endif