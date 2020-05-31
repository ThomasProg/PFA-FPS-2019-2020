#include "staticObject.hpp"

void Entity::StaticObject::setResources(const DemoResourceManager& resourceManager,                                   
                                        E_Model model, 
                                        E_Shader shader,
                                        E_Texture texture)
{
    mesh.model   = &resourceManager.get(model);
    mesh.shader  = &resourceManager.get(shader);
    mesh.texture = &resourceManager.get(texture);
    mesh.linkShaderWithModel();
}
