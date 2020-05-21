#include "renderableInterface.hpp"

#include "enumedResourceManager.hpp"

// void Renderer::RenderableInterface::setResources(const EnumedResourceManager& resourceManager)
// {
//     mesh.model   = &resourceManager.get(E_Model::E_BOX);
//     mesh.shader  = &resourceManager.get(E_Shader::E_LIGHTED);
//     mesh.texture = &resourceManager.get(E_Texture::E_GROUND);
//     mesh.linkShaderWithModel();
// }

void Renderer::RenderableInterface::addRendering(Renderer::RendererSystem& renderer)
{
    meshIt = renderer.addComponent(&mesh);
}

