#include <iostream>
#include "../../include/primitives/primitives.hpp"
#include "vec3.hpp"
#include <cmath>

Core::Maths::Vec3 sp(int i, int j, int l, int L)
{
    float r = sin(i*(M_PI/l));
    return {(float)cos(i*(M_PI/l)), (float)cos(j*(2*M_PI/L)) * r, (float)sin(j*(2*M_PI/L)) * r};
}

Primitives Primitives::CreateSphere(unsigned int l, unsigned int L)
{
    std::vector<Core::Maths::Vec3>   vertices;
    vertices.reserve(4*L*l);
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < l; i++)
    {
        for (unsigned int j = 0; j < L; j++)
        {
            Core::Maths::Vec3 v1 = sp(i, j, l, L);
            indices.push_back(vertices.size());
            vertices.push_back(v1);
           
            Core::Maths::Vec3 v2 = sp(i+1, j, l, L);
            indices.push_back(vertices.size());
            vertices.push_back(v2);
           
            Core::Maths::Vec3 v3 = sp(i, j+1, l, L);
            indices.push_back(vertices.size());
            vertices.push_back(v3);  

            indices.push_back(vertices.size()-1);
            indices.push_back(vertices.size()-2);
    
            Core::Maths::Vec3 v6 = sp(i+1, j+1, l, L);
            indices.push_back(vertices.size());
            vertices.push_back(v6);    
        }
    }

    return Primitives{std::move(vertices), {}, std::move(indices)};
}

Primitives Primitives::CreateCube()
{
    std::vector<Core::Maths::Vec3> vertices;
    std::vector<Core::Maths::Vec3> normals;
    std::vector<Core::Maths::Vec2> uvs;
    std::vector<unsigned int> indices;

    vertices.reserve(8);

    Core::Maths::Vec3 v1 (-1, 1, 1); // bas gauche up
    vertices.push_back(v1);

    Core::Maths::Vec3 v2 (-1, 1, -1); // bas gauche down
    vertices.push_back(v2);

    Core::Maths::Vec3 v3 (-1, -1, 1); // haut gauche up
    vertices.push_back(v3);

    Core::Maths::Vec3 v4 (-1, -1, -1); // haut gauche down
    vertices.push_back(v4);

    Core::Maths::Vec3 v5 (1, 1, 1); // bas droite up
    vertices.push_back(v5);  
    
    Core::Maths::Vec3 v6 (1, -1, 1); // haut droite up
    vertices.push_back(v6);

    Core::Maths::Vec3 v7 (1, -1, -1); // haut droite down
    vertices.push_back(v7);
    
    Core::Maths::Vec3 v8 (1, 1, -1); // bas droite down
    vertices.push_back(v8);
    
    indices.reserve(36);
    uvs.reserve(36);

    // Left Side
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(3);

    for (unsigned int i = 0; i < 6; i++)
        normals.emplace_back(-1, 0, 0);

    uvs.emplace_back(1.f, 1.f);
    uvs.emplace_back(1.f, 0.f);
    uvs.emplace_back(0.f, 1.f);
    uvs.emplace_back(0.f, 1.f);
    uvs.emplace_back(1.f, 0.f);
    uvs.emplace_back(0.f, 0.f);

    // First Face (back)
    indices.push_back(4);
    indices.push_back(0);
    indices.push_back(5);
    indices.push_back(5);
    indices.push_back(0);
    indices.push_back(2);

    for (unsigned int i = 0; i < 6; i++)
        normals.emplace_back(0, 0, 1);

    uvs.emplace_back(1.f, 1.f);
    uvs.emplace_back(1.f, 0.f);
    uvs.emplace_back(0.f, 1.f);
    uvs.emplace_back(0.f, 1.f);
    uvs.emplace_back(1.f, 0.f);
    uvs.emplace_back(0.f, 0.f);

    // Second Face (bottom)
    indices.push_back(3);
    indices.push_back(5);
    indices.push_back(2);
    indices.push_back(6);
    indices.push_back(5);
    indices.push_back(3);

    for (unsigned int i = 0; i < 6; i++)
        normals.emplace_back(0, -1, 0);

    uvs.emplace_back(1.f, 0.f);
    uvs.emplace_back(0.f, 1.f);
    uvs.emplace_back(0.f, 0.f);
    uvs.emplace_back(1.f, 1.f);
    uvs.emplace_back(0.f, 1.f);
    uvs.emplace_back(1.f, 0.f);

    // Third Face (front)
    indices.push_back(6);
    indices.push_back(1);
    indices.push_back(7);
    indices.push_back(3);
    indices.push_back(1);
    indices.push_back(6);

    for (unsigned int i = 0; i < 6; i++)
        normals.emplace_back(0, 0, -1);

    uvs.emplace_back(1.f, 0.f);
    uvs.emplace_back(0.f, 1.f);
    uvs.emplace_back(0.f, 0.f);
    uvs.emplace_back(1.f, 1.f);
    uvs.emplace_back(0.f, 1.f);
    uvs.emplace_back(1.f, 0.f);

    // Fourth Face (Top)
    indices.push_back(4);
    indices.push_back(7);
    indices.push_back(1);
    indices.push_back(4);
    indices.push_back(1);
    indices.push_back(0);

    for (unsigned int i = 0; i < 6; i++)
        normals.emplace_back(0, 1, 0);

    uvs.emplace_back(0.f, 1.f);
    uvs.emplace_back(1.f, 1.f);
    uvs.emplace_back(1.f, 0.f);
    uvs.emplace_back(0.f, 1.f);
    uvs.emplace_back(1.f, 0.f);
    uvs.emplace_back(0.f, 0.f);

    // Right Side
    indices.push_back(7);
    indices.push_back(4);
    indices.push_back(6);
    indices.push_back(6);
    indices.push_back(4);
    indices.push_back(5);

    for (unsigned int i = 0; i < 6; i++)
        normals.emplace_back(1, 0, 0);

    uvs.emplace_back(1.f, 1.f);
    uvs.emplace_back(1.f, 0.f);
    uvs.emplace_back(0.f, 1.f);
    uvs.emplace_back(0.f, 1.f);
    uvs.emplace_back(1.f, 0.f);
    uvs.emplace_back(0.f, 0.f);

    return Primitives{std::move(vertices), std::move(normals), std::move(indices), std::move(uvs)};
}

Primitives Primitives::CreateTriangle()
{
    std::vector<Core::Maths::Vec3>   vertices;
    vertices.reserve(3);
    std::vector<unsigned int> indices;

    Core::Maths::Vec3 v1 (0.f, 0.5f, 0.f);
    vertices.push_back(v1);
    indices.push_back(1);

    Core::Maths::Vec3 v2 (-0.5f, -0.5f, 0.f);
    vertices.push_back(v2);
    indices.push_back(2);

    Core::Maths::Vec3 v3 (0.5f, -0.5f, 0.f);
    vertices.push_back(v3);
    indices.push_back(3);

    return Primitives{std::move(vertices), {}, std::move(indices)};
}

