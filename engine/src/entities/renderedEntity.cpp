#include "renderedEntity.hpp"

void Entity::RenderedEntity::setResources(const DemoResourceManager& resourceManager)
{
    mesh.model   = &resourceManager.get(E_Model::E_BOX);
    mesh.shader  = &resourceManager.get(E_Shader::E_LIGHTED_FLATCOLOR);
    mesh.linkShaderWithModel();
}
