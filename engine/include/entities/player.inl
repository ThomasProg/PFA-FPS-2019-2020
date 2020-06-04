#include "player.hpp"

inline void Entity::Player::addRendering(Renderer::RendererSystem& rendererSystem)
{
    gun.addRendering(rendererSystem);
}

inline void Entity::Player::removeRendering(Renderer::RendererSystem& rendererSystem)
{
    rendererSystem.erase(gun.meshIt);
}

inline void Entity::Player::physicCompOnCollisionEnter(const Physics::Shapes::SegmentHit& hit, CollisionComponentInterface<Physics::Shapes::Box>* otherCollider) 
{
    if (hit.normal.y > 0.5) 
        currentGround = otherCollider;
}

inline void Entity::Player::physicCompOnCollisionExit(CollisionComponentInterface<Physics::Shapes::Box>* otherCollider) 
{
    if (currentGround == otherCollider)
        currentGround = nullptr;
}

inline void Entity::Player::physicCompOnOverlapEnter   (const Physics::PhysicsSystem::CollisionsCallbacksSentData& data)
{
    if (data.encounteredEntityID == this)
        return;
}

inline void Entity::Player::colliderOnOverlapEnter   (const Physics::PhysicsSystem::CollisionsCallbacksSentData& data)
{
    if (data.movingEntityID == this)
        return;
}