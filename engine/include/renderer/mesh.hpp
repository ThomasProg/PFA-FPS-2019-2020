#ifndef _MESH_HPP_
#define _MESH_HPP_

#include <vector>

#include "gTransform.hpp"
#include "camera.hpp"

#include "lightManager.hpp"

namespace Resources
{
    class Model;
    class Shader;
    class Texture;
}

namespace Renderer
{
    class Mesh 
    {
        // TODO : let public?
    public:
        const Resources::Model*  model  = nullptr;
        const Resources::Shader* shader = nullptr;
        const Resources::Texture* texture = nullptr;
        Physics::GTransform* transform = nullptr;

        bool isDrawn = true;

        Core::Maths::Vec4 color = {1,0,0,1};

    public:

        void linkShaderWithModel();

        void draw(const Renderer::Camera&, Renderer::LightManager& lightManager);
    };
}

#endif