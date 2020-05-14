#include "enemy.hpp"

inline void Entity::Enemy::setup(Renderer::RendererSystem& renderer, 
                    const Resources::Model* model, 
                    const Resources::Shader* shader,
                    const Resources::Texture* texture,
                    Physics::TransformGraph& transformParent)
{
    BasicEntity::setup(renderer, model, shader, texture, transformParent);
}

inline void Entity::Enemy::setup2(const Core::Maths::Vec3& patrolTarget, const Core::Maths::Vec3& chaseTarget)
{
    this->patrolTarget = patrolTarget;
    this->chaseTarget = chaseTarget;
}