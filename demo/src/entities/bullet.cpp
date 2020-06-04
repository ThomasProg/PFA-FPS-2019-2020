#include "bullet.hpp"

Entity::Bullet::Bullet()
    : RenderableInterface(&transform)
{
    mesh.color = defaultColor;
}

void Entity::Bullet::setResources(const DemoResourceManager& resourceManager)
{
    mesh.model  = &resourceManager.get(defaultModel);
    mesh.shader = &resourceManager.get(defaultShader);
    mesh.linkShaderWithModel();
}
