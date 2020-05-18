#ifndef _MESH_HPP_
#define _MESH_HPP_

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
    class Mesh 
    {
        // TODO : let public?
    public:
        const Resources::Model*  model  = nullptr;
        const Resources::Shader* shader = nullptr;
        const Resources::Texture* texture = nullptr;
        Physics::GTransform* transform;

        bool isDrawn = true;

        Core::Maths::Vec4 color = {1,0,0,1};

    public:
        // Callback called when draw() is called.
        // Can be used to modify buffer access (color / depth / stencil buffers).
        // If you want to modify the GTransform of an object with this callback,
        // make sure you call cleanUpdate() or it won't take effect.
        std::function<void()> onDraw = nullptr;

        void linkShaderWithModel();

        void draw(const Renderer::Camera&);
    };
}

#endif