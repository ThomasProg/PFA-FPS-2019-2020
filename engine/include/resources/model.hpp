#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include "vec2.hpp"
#include "vec3.hpp"
#include <vector>

class Primitives;

namespace Resources
{
    class Model
    {
    public:
        // Index to the Vertex Array Object from opengl.
        unsigned int vao = 0;
        // Index to the Vertex Buffer Object from opengl.
        unsigned int vbo = 0;

        std::vector<Core::Maths::Vec3> positions;
        std::vector<Core::Maths::Vec3> normals;
        std::vector<Core::Maths::Vec2> textCoords;

        Model() = default;
        Model(const char* pathToFile);

        Model(Primitives&& primitive);
        Model(Model&&);

        // We don't want to copy the opengl data
        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

        // Free resources from opengl
        ~Model();

        void loadOBJ(const char* pathToFile);
        void setupModel();
    };
}

#endif