#include "cylinder.hpp"

inline std::ostream& Physics::Shapes::operator<<(std::ostream& stream, const Physics::Shapes::Cylinder& cylinder)
{
    stream << "p1 : " << cylinder.location << '\t' << "p2 : " << cylinder.dirInWorldLoc << '\t' << "Radius : " << cylinder.radius;
    return stream;
}

