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

    physicComponent.collider.onCollisionEnter = [this](SegmentHit&)
    {
        // std::cout << "collision enter" << std::endl;
        state.playerState = PlayerState::E_IDLE;
    };

    physicComponent.collider.onCollisionExit = [this]()
    {
        // std::cout << "collision exit" << std::endl;
        state.playerState = PlayerState::E_JUMPING;
    };
}

void Entity::Player::setup(Renderer::RendererSystem& renderer, 
            const Resources::Model* model, 
            const Resources::Shader* shader,
            const Resources::Texture* texture, 
            Physics::TransformGraph& transformParent) 
{
    BasicEntity::setup(renderer, model, shader, texture, transformParent);
    camera.setup(mesh->transform);

    physicComponent.collider.onCollisionEnter = [this](SegmentHit&)
    {
        // std::cout << "collision enter" << std::endl;
        state.playerState = PlayerState::E_IDLE;
    };

    physicComponent.collider.onCollisionExit = [this]()
    {
        // std::cout << "collision exit" << std::endl;
        state.playerState = PlayerState::E_JUMPING;
    };
}

void Entity::Player::inputs(const Core::Engine& engine)
{       
    if (state.isOnGround() && glfwGetKey(engine.window, inputKeys.jump))
    {
        physicComponent.velocity.y = jumpSpeed;
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
            physicComponent.velocity.x = addedVelocity.x;
            physicComponent.velocity.z = addedVelocity.z;
        }
    }
    // else
    // {
    //     physicComponent.velocity.x = 0.f;
    //     physicComponent.velocity.z = 0.f;        
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


bool Entity::Player::dealDamages(float damages)
{
    lifePoints -= damages;
    return (lifePoints <= 0.f);
}