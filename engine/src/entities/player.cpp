#include "player.hpp"
#include "basicEntity.hpp"

#include "utilities.hpp"

#include "segment3D.hpp"
#include "engine.hpp"

#define _IS_MOUSE_ENABLED_ 1

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
    camera.setup(transform);

    camera.transform.transform.location.y = 2.f;
    camera.transform.UpdateLocalTransformMatrix();
    camera.transform.transformMatrixNode->setDirtySelfAndChildren();
}

void Entity::Player::setup(Renderer::RendererSystem& renderer, 
            const Resources::Model* model, 
            const Resources::Shader* shader,
            const Resources::Texture* texture, 
            Physics::TransformGraph& transformParent) 
{
    BasicEntity::setup(renderer, model, shader, texture, transformParent);
    camera.setup(transform);

    camera.transform.transform.location.y = 2.f;
    camera.transform.UpdateLocalTransformMatrix();
    camera.transform.transformMatrixNode->setDirtySelfAndChildren();
}

void Entity::Player::inputs(const Core::Engine& engine)
{       
    // if (!physicCompIt.isValid())
    //     return;
    
    std::vector<unsigned int>::iterator it;

    // if (state.isOnGround() && glfwGetKey(engine.window, inputKeys.jump))
    if (physicComp.collider.collidingEntities.size() > 0 && glfwGetKey(engine.window, inputKeys.jump))
    {
        physicComp.setForceOnAxis<1>(jumpSpeed, engine);
        // state.playerState = PlayerState::E_JUMPING;
    }

    Core::Maths::Vec3 forward = transform.transform.getForwardXZVector() * movementSpeed;
    Core::Maths::Vec3 right   = transform.transform.getRightXZVector()   * movementSpeed;

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
            physicComp.setForceOnAxis<0>(addedVelocity.x, engine);
            physicComp.setForceOnAxis<2>(addedVelocity.z, engine);
        }
    }
    // else
    // {
    //     physicComp.velocity.x = 0.f;
    //     physicComp.velocity.z = 0.f;        
    // }

    // camera.inputs(engine);

    constexpr float mouseSensibility = 10.f;
    constexpr float rotationSpeedOnKey = 3.f;
    constexpr float rotationSpeed = 0.1;

    #if _IS_MOUSE_ENABLED_
    float deltaMouseX = - engine.deltaMouseLoc.x * mouseSensibility;
    float deltaMouseY = - engine.deltaMouseLoc.y * mouseSensibility; 
    #else
    float deltaMouseX = 0;
    float deltaMouseY = 0;
    #endif

    if (glfwGetKey(engine.window, GLFW_KEY_I))
        deltaMouseY = rotationSpeedOnKey;
    if (glfwGetKey(engine.window, GLFW_KEY_J))
        deltaMouseX = rotationSpeedOnKey;
    if (glfwGetKey(engine.window, GLFW_KEY_K))
        deltaMouseY = -rotationSpeedOnKey;
    if (glfwGetKey(engine.window, GLFW_KEY_L))
        deltaMouseX = -rotationSpeedOnKey;

    transform.transform.rotation.y += deltaMouseX * rotationSpeed;
    camera.transform.transform.rotation.x = clamp(camera.transform.transform.rotation.x + deltaMouseY * rotationSpeed, float(- M_PI / 2.f), float(M_PI / 2.f));

    transform.UpdateLocalTransformMatrix();
    transform.transformMatrixNode->setDirtySelfAndChildren();
    transform.transformMatrixNode->cleanUpdate();

    camera.transform.UpdateLocalTransformMatrix();
    camera.transform.transformMatrixNode->setDirtySelfAndChildren(); 
    camera.transform.transformMatrixNode->cleanUpdate();
}

bool Entity::Player::isShooting(const Core::Engine& engine)
{
    return engine.isMouseButtonDown(inputKeys.fire);
}

Segment3D Entity::Player::shoot() const
{
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



// void Entity::Player::onCollisionEnter(const SegmentHit& hit) 
// {
//     state.playerState = PlayerState::E_IDLE;
// }

// void Entity::Player::onCollisionExit() 
// {
//     state.playerState = PlayerState::E_JUMPING;
// }

// void Entity::Player::onOverlapEnterSelfHit(const SegmentHit& hit) 
// {
//     if (hit.normal.y < 0.5)
//     {
//         dealDamages(1.f);
//     }
// }

// void Entity::Player::onOverlapEnterAnotherHit(const SegmentHit& hit) 
// {
//     if (hit.normal.y > - 0.5)
//     {
//         dealDamages(1.f);
//     }
// }

// void Entity::Player::onOverlapEnter(const Physics::PhysicsSystem::CollisionsCallbacksSentData& data) 
// {
        
// }
