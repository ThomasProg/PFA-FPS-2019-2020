#include "ground.hpp"
#include "demoResourceManager.hpp"

Entity::Ground::Ground() 
    : Renderer::RenderableInterface(&transform),
        Physics::CollisionComponentInterface<Physics::Shapes::Box>(&transform)
{
    mesh.color = Core::Maths::Vec4{0.835f, 0.650f, 0.384f,1};
    mesh.color   = {0.301f, 0.113f, 0.f, 1};
}

Entity::Ground::Ground(const Core::Maths::Vec4& color) 
    : Renderer::RenderableInterface(&transform),
        Physics::CollisionComponentInterface<Physics::Shapes::Box>(&transform)
{
    mesh.color = color;
}

void Entity::Ground::setResources(const DemoResourceManager& resourceManager)
{
    mesh.model   = &resourceManager.get(E_Model::E_BOX);
    mesh.shader  = &resourceManager.get(E_Shader::E_LIGHTED_FLATCOLOR);
    mesh.linkShaderWithModel();
}
