#ifndef _CYLINDER_HPP_
#define _CYLINDER_HPP_

#include "vec3.hpp"

struct Cylinder
{
    Core::Maths::Vec3 location;
    Core::Maths::Vec3 dirInWorldLoc;
    float radius;
};

inline std::ostream& operator<<(std::ostream& stream, const Cylinder& cylinder)
{
    stream << "p1 : " << cylinder.location << '\t' << "p2 : " << cylinder.dirInWorldLoc << '\t' << "Radius : " << cylinder.radius;
    return stream;
}

#endif