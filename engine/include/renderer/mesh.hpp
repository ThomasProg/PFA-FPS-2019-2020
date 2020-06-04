#ifndef _MESH_HPP_
#define _MESH_HPP_

#include <vector>

#include "gTransform.hpp"
#include "camera.hpp"

namespace Resources
{
    class Model;
    class Shader;
    class Texture;
}

namespace Renderer
{
    class LightManager;
}

namespace Renderer
{
    class Mesh 
    {
    public:
        const Resources::Model*  model  = nullptr;
        const Resources::Shader* shader = nullptr;
        const Resources::Texture* texture = nullptr;
        Physics::GTransform* transform = nullptr;

        bool isDrawn = true;

        Core::Maths::Vec4 color = {1,1,1,1};

    public:

        Mesh(Physics::GTransform* transform);

        void linkShaderWithModel();

        void draw(const Renderer::Camera&, Renderer::LightManager& lightManager);
    };
}

#endif