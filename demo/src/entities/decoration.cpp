#include "decoration.hpp"


void Entity::Decoration::setResources(const DemoResourceManager& resourceManager,
                                E_Model model, 
                                E_Shader shader)
{
    mesh.model   = &resourceManager.get(model);
    mesh.shader  = &resourceManager.get(shader);
    mesh.linkShaderWithModel();
}