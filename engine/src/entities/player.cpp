#include "player.hpp"

#include "utilities.hpp"

#include "segment3D.hpp"
#include "engine.hpp"

#include "enemy.hpp"
#include "ground.hpp"
#include "physicsSystem.hpp"
#include "entityGroup.hpp"

#include "layersEnum.hpp"

#define _IS_MOUSE_ENABLED_ 1

Entity::Player::Player() 
    : //Renderer::RenderableInterface(&transform),
        Physics::CollisionComponentInterface<Physics::Shapes::Box>(&transform),
        Physics::PhysicComponentInterface(&transform)
{
    collider.isOverlap = true;
    physicComp.collider.worldCollider.radius = 1.f;
    collider.layers = LayersEnum::E_PLAYER;
}

void Entity::Player::setTransformParent(Physics::TransformGraph& transformParent)
{
    TransformInterface::setTransformParent(transformParent);

    camera.setTransformParent(*this);

    gun.setTransformParent(camera);
}

void Entity::Player::setTransform(const Physics::Transform& newTransform)
{
    TransformInterface::setTransform(newTransform);
    
    camera.setTransform({{0,1,0}, {0.f,0.f,0.f}, {1,1,1}});

    gun.setTransform({{1.f,0.2f,0.f}, {0.f,0.f,0.f}, {1,1,1}});
}

bool Entity::PlayerState::isOnGround() const noexcept
{
    return playerState == E_IDLE || playerState == E_WALKING;
}

void Entity::Player::setResources(const DemoResourceManager& resourceManager)
{
    gun.mesh.model   = &resourceManager.get(E_Model::E_GUN);
    gun.mesh.shader  = &resourceManager.get(E_Shader::E_LIGHTED);
    gun.mesh.texture = &resourceManager.get(E_Texture::E_GUN);
    gun.mesh.linkShaderWithModel();

    audio.setAudio(resourceManager.get(E_Audio::E_SHOOT));
}

void Entity::Player::inputs(const Core::Engine& engine)
{       
    std::array<bool, nbInputKeys> keys = getDownKeys(engine);

    if (keys[E_JUMP])
        lastJumpPressTime = glfwGetTime();

    Core::Maths::Vec3 forward = transform.transform.getForwardXZVector() * movementSpeed;
    Core::Maths::Vec3 right   = transform.transform.getRightXZVector()   * movementSpeed;

    Core::Maths::Vec3 addedVelocity;

    bool hasMoved = false;

    if (keys[E_FORWARD])
    {
        addedVelocity += forward; 
        hasMoved = true;
    }
    if (keys[E_BACKWARD])
    {
        addedVelocity += - forward; 
        hasMoved = true;
    }
    if (keys[E_RIGHT])
    {
        addedVelocity += right; 
        hasMoved = true;
    }
    if (keys[E_LEFT])
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
            physicComp.setVelocityOnAxis<0>(addedVelocity.x, engine);
            physicComp.setVelocityOnAxis<2>(addedVelocity.z, engine);
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
    camera.transform.UpdateLocalTransformMatrix();
}

void Entity::Player::tryToJump(const Core::Engine& engine)
{
    if (currentGround && glfwGetTime() - lastJumpPressTime < jumpCoyoteTime)
    {
        physicComp.velocity.y = jumpSpeed; // impulse
        state.playerState = PlayerState::E_JUMPING;
    }
}


Physics::Shapes::Segment3D Entity::Player::getShootRay() const
{
    Physics::Shapes::Segment3D seg;

    seg.p1 = {0.f, 0, 0};
    seg.p2 = {0.f, 0, -shootRayLength};
    seg.p1 = Core::Maths::Vec3{camera.transform.transformMatrixNode->getWorldMatrix() * Core::Maths::Vec4{seg.p1, 1}};
    seg.p2 = Core::Maths::Vec3{camera.transform.transformMatrixNode->getWorldMatrix() * Core::Maths::Vec4{seg.p2, 1}};

    return seg;
}

void Entity::Player::shoot(Physics::PhysicsSystem& physicsSystem, EntityGroup& entityGroup, float playTime)
{
    if (nbBullet > 0)
    {
        if (playTime - lastShootTime >= shootCooldown)
        {
            audio.play();
            nbBullet--;
            lastShootTime = playTime;

            Physics::Shapes::Segment3D directionHit = getShootRay();
            Physics::Shapes::SegmentHit hit;
            Physics::CollisionComponentInterface<Physics::Shapes::Box>* touchEntity = nullptr;

            if(physicsSystem.raycast(directionHit, hit, touchEntity, ~LayersEnum::E_PLAYER))
            {
                //test hit enemy
                std::vector<std::unique_ptr<Entity::Enemy>>::iterator it = entityGroup.enemies.begin();
                while (it != entityGroup.enemies.end() && (*it).get() != touchEntity)
                {
                    it++;
                }
                if (it != entityGroup.enemies.end())
                {
                    //hit: add a box during 2s
                    (*it)->takeDamage(5, playTime);
                    entityGroup.addBullet({{hit.collisionPoint}, {0,0,0.f}, {0.05f,0.05f,0.05f}});
                }
                else 
                {
                    //test hit ground/wall
                    std::vector<std::unique_ptr<Entity::Ground>>::iterator it = entityGroup.grounds.begin();
                    while (it != entityGroup.grounds.end() && (*it).get() != touchEntity)
                    {
                        it++;
                    }
                    if (it != entityGroup.grounds.end())
                    {
                        entityGroup.addBullet({{hit.collisionPoint}, {0,0,0.f}, {0.1f,0.1f,0.1f}});
                    }
                }
            }
        }
    }
}

void Entity::Player::dealDamages(float damages)
{
    lifePoints -= damages;
    if (/*onPlayerDeath && */lifePoints <= 0.f)
    {
        state.playerState = PlayerState::E_DEAD;
        //onPlayerDeath();
        gOver = true;
        lifePoints = 0;
    }
}

void Entity::Player::reloadAmmo()
{
    nbBullet = maxNbBullet;
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


std::array<bool, Entity::Player::nbInputKeys> Entity::Player::getDownKeysAzertyAndQwery(const Core::Engine& engine)
{
    std::array<bool, nbInputKeys> keys;

    keys[E_FORWARD]  = (engine.isKeyDown(GLFW_KEY_W) || engine.isKeyDown(GLFW_KEY_Z));
    keys[E_LEFT]     = (engine.isKeyDown(GLFW_KEY_A) || engine.isKeyDown(GLFW_KEY_Q));
    keys[E_BACKWARD] = (engine.isKeyDown(GLFW_KEY_S));
    keys[E_RIGHT]    = (engine.isKeyDown(GLFW_KEY_D));

    keys[E_JUMP] = (engine.isKeyDown(GLFW_KEY_SPACE));

    keys[E_FIRE] = (engine.isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT));

    return keys;
}
