#include "ground.hpp"
#include "demoResourceManager.hpp"

constexpr Core::Maths::Vec4 Entity::Ground::defaultColor;

Entity::Ground::Ground() 
    : Renderer::RenderableInterface(&transform),
        Physics::CollisionComponentInterface<Physics::Shapes::Box>(&transform)
{
    mesh.color = defaultColor;
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
