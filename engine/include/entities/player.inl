#include "player.hpp"

inline void Entity::Player::addRendering(Renderer::RendererSystem& rendererSystem)
{
    gun.addRendering(rendererSystem);
}

inline void Entity::Player::removeRendering(Renderer::RendererSystem& rendererSystem)
{
    rendererSystem.erase(gun.meshIt);
}
