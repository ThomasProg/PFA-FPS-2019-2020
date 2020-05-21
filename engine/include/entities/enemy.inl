#include "enemy.hpp"

// inline void Entity::Enemy::setup(Renderer::RendererSystem& renderer, 
//                     const Resources::Model* model, 
//                     const Resources::Shader* shader,
//                     const Resources::Texture* texture,
//                     Physics::TransformGraph& transformParent)
// {
//     BasicEntity::setup(renderer, model, shader, texture, transformParent);

//     physicComp.collider.worldCollider.radius = 1.f;
// }

inline void Entity::Enemy::raycastCollide()
{
    isDead = true;
}