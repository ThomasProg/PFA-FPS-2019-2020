#ifndef _PRIMITIVES_HPP_
#define _PRIMITIVES_HPP_

#include <vector>

#include "vec3.hpp"
#include "vec2.hpp"

class Primitives
{
public:
    Primitives() = default;
    ~Primitives() = default;

    std::vector<Core::Maths::Vec3> vertices;
    std::vector<Core::Maths::Vec3> normals;
    std::vector<unsigned int> indices;
    std::vector<Core::Maths::Vec2> uvs;

    static Primitives CreateTriangle();
    static Primitives CreateCube();
    static Primitives CreateSphere(unsigned int latitudeCount, unsigned int longitudeCount);
};


#endif