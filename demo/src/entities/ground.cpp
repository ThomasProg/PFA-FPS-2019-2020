#include "ground.hpp"
#include "demoResourceManager.hpp"


void Entity::Ground::setResources(const DemoResourceManager& resourceManager)
{
    mesh.model   = &resourceManager.get(E_Model::E_BOX);
    mesh.shader  = &resourceManager.get(E_Shader::E_FLAT);
    mesh.color   = Core::Maths::Vec4{0.835f, 0.650f, 0.384f,1};
    mesh.texture = &resourceManager.get(E_Texture::E_GROUND);
    mesh.linkShaderWithModel();
}
