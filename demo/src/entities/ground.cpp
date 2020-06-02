#include "ground.hpp"
#include "demoResourceManager.hpp"


void Entity::Ground::setResources(const DemoResourceManager& resourceManager)
{
    mesh.model   = &resourceManager.get(E_Model::E_BOX);
    mesh.shader  = &resourceManager.get(E_Shader::E_LIGHTED_FLATCOLOR);
    mesh.texture = &resourceManager.get(E_Texture::E_GROUND);
    mesh.linkShaderWithModel();
}
