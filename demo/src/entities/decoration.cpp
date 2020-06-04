#include "decoration.hpp"

Entity::Decoration::Decoration() 
    : Renderer::RenderableInterface(&transform)
{
    mesh.color = Core::Maths::Vec4{0.1f, 0.450f, 0.1f,1};
}

Entity::Decoration::Decoration(const Core::Maths::Vec4& color) 
    : Renderer::RenderableInterface(&transform)
{
    mesh.color = color;
}

void Entity::Decoration::setResources(const DemoResourceManager& resourceManager,
                                E_Model model, 
                                E_Shader shader)
{
    mesh.model   = &resourceManager.get(model);
    mesh.shader  = &resourceManager.get(shader);
    mesh.linkShaderWithModel();
}