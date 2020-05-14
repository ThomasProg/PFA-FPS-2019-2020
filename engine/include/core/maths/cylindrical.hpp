#ifndef _CYLINDRICAL_HPP_
#define _CYLINDRICAL_HPP_

#include "vec3.hpp"

namespace Core::Maths
{
    struct Cylindrical
    {
    public:
        float length, theta, z;

    public:
        static Cylindrical convertToCylindrical(Core::Maths::Vec3 cartesianCoords)
        {
            float length = std::sqrt(cartesianCoords.x * cartesianCoords.x + cartesianCoords.y * cartesianCoords.y);

            return { length,
                     cartesianCoords.y >= 0.f ? std::acos(cartesianCoords.x / length) 
                                            : 2.f * float(M_PI) - std::acos(cartesianCoords.x / length),
                     cartesianCoords.z }; 
        }

        static Core::Maths::Vec3 convertToCartesian(Cylindrical cylindricalCoords)
        {
            return { cylindricalCoords.length * std::cos(cylindricalCoords.theta), 
                     cylindricalCoords.length * std::sin(cylindricalCoords.theta), 
                     cylindricalCoords.z };
        }
    };
}

#endif