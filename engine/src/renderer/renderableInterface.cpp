#include "renderableInterface.hpp"

Renderer::RenderableInterface::RenderableInterface(Physics::GTransform* transform)
    : mesh(transform)
{

}

void Renderer::RenderableInterface::addRendering(Renderer::RendererSystem& renderer)
{
    meshIt = renderer.addComponent(&mesh);
}

