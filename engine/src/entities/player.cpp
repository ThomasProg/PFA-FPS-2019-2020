#include "player.hpp"
#include "basicEntity.hpp"

bool Entity::PlayerState::isOnGround() const noexcept
{
    return playerState == E_IDLE || playerState == E_WALKING;
}

void Entity::Player::setup(Renderer::RendererSystem& renderer, 
            const Resources::Model* model, 
            const Resources::Shader* shader, 
            Physics::TransformGraph& transformParent) 
{
    BasicEntity::setup(renderer, model, shader, transformParent);
    camera.setup(mesh->transform);
}

void Entity::Player::setup(Renderer::RendererSystem& renderer, 
            const Resources::Model* model, 
            const Resources::Shader* shader,
            const Resources::Texture* texture, 
            Physics::TransformGraph& transformParent) 
{
    BasicEntity::setup(renderer, model, shader, texture, transformParent);
    camera.setup(mesh->transform);
}

void Entity::Player::inputs(const Core::Engine& engine)
{       
    if (state.isOnGround() && glfwGetKey(engine.window, inputKeys.jump))
    {
        physicCompIt->velocity.y = jumpSpeed;
        // state.playerState = PlayerState::E_JUMPING;
    }

    Core::Maths::Vec3 forward = camera.camAnchor.transform.getForwardXZVector() * (engine.deltaTime * movementSpeed);
    Core::Maths::Vec3 right   = camera.camAnchor.transform.getRightXZVector()   * (engine.deltaTime * movementSpeed);

    Core::Maths::Vec3 addedVelocity;

    bool hasMoved = false;

    if (engine.isKeyDown(inputKeys.forward))
    {
        addedVelocity += forward; 
        hasMoved = true;
    }
    if (engine.isKeyDown(inputKeys.backward))
    {
        addedVelocity += - forward; 
        hasMoved = true;
    }
    if (engine.isKeyDown(inputKeys.right))
    {
        addedVelocity += right; 
        hasMoved = true;
    }
    if (engine.isKeyDown(inputKeys.left))
    {
        addedVelocity += - right; 
        hasMoved = true;
    }
    /*if(engine.isMouseButtonDown(inputKeys.fire))
    {
        //shoot();
    }*/

    if (hasMoved)
    {
        // should not be 0, since it has moved
        if (addedVelocity.vectorSquareLength() != 0)
        {
            physicCompIt->velocity.x = addedVelocity.x;
            physicCompIt->velocity.z = addedVelocity.z;
        }
    }
    // else
    // {
    //     physicCompIt->velocity.x = 0.f;
    //     physicCompIt->velocity.z = 0.f;        
    // }

    camera.inputs(engine);
}

Segment3D Entity::Player::shoot() const
{
    Core::Maths::Vec3 direction;
    Segment3D seg;

    seg.p1 = {0.f, 0, 0};
    seg.p2 = {0.f, 0, -100};
    seg.p1 = camera.transform.transformMatrixNode->worldData * seg.p1;
    seg.p2 = camera.transform.transformMatrixNode->worldData * seg.p2;

    return seg;
}


void Entity::Player::dealDamages(float damages)
{
    lifePoints -= damages;
    if (onPlayerDeath && lifePoints <= 0.f)
    {
        onPlayerDeath();
    }
}



void Entity::Player::onCollisionEnter(const SegmentHit& hit) 
{
    state.playerState = PlayerState::E_IDLE;
}

void Entity::Player::onCollisionExit() 
{
    state.playerState = PlayerState::E_JUMPING;
}

void Entity::Player::onOverlapEnterSelfHit(const SegmentHit& hit) 
{
    if (hit.normal.y < 0.5)
    {
        dealDamages(1.f);
    }
}

void Entity::Player::onOverlapEnterAnotherHit(const SegmentHit& hit) 
{
    if (hit.normal.y > - 0.5)
    {
        dealDamages(1.f);
    }
}