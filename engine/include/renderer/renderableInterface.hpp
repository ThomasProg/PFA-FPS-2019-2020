#ifndef _RENDERABLE_INTERFACE_HPP_
#define _RENDERABLE_INTERFACE_HPP_

#include "mesh.hpp"
#include "rendererSystem.hpp"

namespace Renderer
{
    class RenderableInterface
    {
    private:

    public:
        Renderer::MeshIt meshIt;
        Renderer::Mesh mesh;

        // void setResources(const EnumedResourceManager&);
        void addRendering(Renderer::RendererSystem& renderer);
    };
}

#endif