#include "player.hpp"

inline void Entity::Player::addRendering(Renderer::RendererSystem& rendererSystem)
{
    gun.addRendering(rendererSystem);
}

inline void Entity::Player::removeRendering(Renderer::RendererSystem& rendererSystem)
{
    rendererSystem.erase(gun.meshIt);
}

inline void Entity::Player::physicCompOnCollisionEnter(const Physics::PhysicsSystem::CollisionsCallbacksSentData& hit) 
{
    if (hit.hit.normal.y > 0.5) 
        currentGround = hit.encounteredEntityID;
}

inline void Entity::Player::physicCompOnCollisionExit(CollisionComponentInterface<Physics::Shapes::Box>* otherCollider) 
{
    if (currentGround == otherCollider)
        currentGround = nullptr;
}
