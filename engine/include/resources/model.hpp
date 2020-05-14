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
    private:
        
    public:
        unsigned int VAO = 0;
        unsigned int VBO = 0;

        std::vector<Core::Maths::Vec3> positions;
        std::vector<Core::Maths::Vec3> normals;
        std::vector<Core::Maths::Vec2> textCoords;
        // std::vector<unsigned int> indices;

        Model() = default;
        Model(const char* pathToFile);
        Model(Primitives&& primitive);
        Model(const Model&) = delete;
        Model(Model&&);
        Model& operator=(const Model&) = delete;
        ~Model();
        void loadOBJ(const char* pathToFile);
        void setupModel();
    };
}

#endif