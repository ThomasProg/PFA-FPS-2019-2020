#include "physicsSystem.hpp"

#include "gTransform.hpp"
#include "engine.hpp"
#include <vector>

// template<typename T = Sphere>
inline Physics::PhysicsSystem::PhysicCompIt Physics::PhysicsSystem::addPhysicComponent(Entity::EntityID& entity)
{
    physicComponents.emplace(entity, Physics::PhysicComponent());
    return Physics::PhysicsSystem::PhysicCompIt{entity, &physicComponents};
}


template<typename COLLISIONS_CALLBACKS>
void Physics::PhysicsSystem::simulate(COLLISIONS_CALLBACKS& callbacks, Core::Engine& engine)
{
    for (std::pair<const Entity::EntityID, Physics::PhysicComponent>& physicComp : physicComponents)
    {
        physicComp.second.collider.worldCollider.center = physicComp.second.collider.transform->transform.location;
        
        for (std::pair<const Entity::EntityID, Physics::CollisionComponent<Box>>& box : boxes)
        {
            box.second.worldCollider.transform = box.second.transform->transformMatrixNode->worldData;
                                            //    * physicComp.second.collider.transform->transformMatrixNode->worldData.getInverse();
        }

        if (physicComp.second.isEnabled)
        {
            Physics::PhysicsSystem::PhysicsAdditionalData playerIgnoreData;
            playerIgnoreData.ignoredEntities.emplace(physicComp.first);
            simulatePhysics(physicComp, 
                            playerIgnoreData, 
                            physicComp.second.velocity,
                            callbacks);
            
            if (physicComp.second.collider.isColliding)
                physicComp.second.velocity *= linearDamping;

            simulateGravity(physicComp.second, engine);

            physicComp.second.collider.transform->UpdateLocalTransformMatrix();
            physicComp.second.collider.transform->transformMatrixNode->cleanUpdate();
        }
    }
}

template<typename COLLISIONS_CALLBACKS>
void Physics::PhysicsSystem::simulatePhysics(std::pair<const Entity::EntityID, Physics::PhysicComponent>& physicComp, 
                                                       const Physics::PhysicsSystem::PhysicsAdditionalData& data,
                                                       const Core::Maths::Vec3& leftVelocity,
                                                       COLLISIONS_CALLBACKS& callbacks)
{
    if (leftVelocity.vectorSquareLength() < 0.001f)
        return;

    // std::cout << leftVelocity << std::endl;
    SegmentHit hit;
    hit.t = 2.f;
    Entity::EntityID collidedEntity; // box

    if (staticBoxesFirstCollision(physicComp.second, physicComp.second.collider.transform->transform.location, hit, data, collidedEntity, leftVelocity))
    {
        Core::Maths::Vec3 newLoc = physicComp.second.collider.transform->transform.location + leftVelocity * hit.t;
        // std::cout << "Old : " << physicComp.second.collider.transform->transform.location << '\t' << " New : " << newLoc << std::endl; 
        // std::cout << "Hit.t : " << hit.t << std::endl;
        staticBoxesOverlapCollision(physicComp.second, 
                                    physicComp.second.collider.transform->transform.location, 
                                    newLoc, 
                                    data, 
                                    physicComp.first, 
                                    callbacks);

        physicComp.second.collider.transform->transform.location = newLoc;

        if (!physicComp.second.collider.isColliding)
        {
            physicComp.second.collider.isColliding = true;

            // Callback
            CollisionsCallbacksSentData collisionsCallbacksSentData
            {
                std::move(hit),
                physicComp.first,
                collidedEntity
            };
            callbacks.onCollisionEnter(collisionsCallbacksSentData);
        }

        Core::Maths::Vec3 newVelo = leftVelocity * hit.t;

        Core::Maths::Vec3 velocityAfterContact = leftVelocity - newVelo;

        // hit.normal = hit.normal.unitVector();
        float dot = Core::Maths::Vec3::dotProduct(hit.normal, velocityAfterContact);

        Core::Maths::Vec3 finalLoc = physicComp.second.collider.transform->transform.location + velocityAfterContact - dot * hit.normal + 0.001f * hit.normal;
        physicComp.second.velocity = (leftVelocity - dot * hit.normal);
        // std::cout << "left : " << physicComp.second.velocity << std::endl;

        // // We add a small value, because otherwises, they player would not be able to move due to constant collisions
        // return finalLoc + hit.normal * minimalDistToGround;
        simulatePhysics(physicComp, data, physicComp.second.velocity, callbacks);
    }
    else 
    {
        if (physicComp.second.collider.isColliding)
        {
            physicComp.second.collider.isColliding = false;

            // Callback
            callbacks.onCollisionExit(physicComp.first);
        }

        physicComp.second.collider.transform->transform.location = physicComp.second.collider.transform->transform.location + leftVelocity;
    }
}

// template<typename COLLISIONS_CALLBACKS>
// void Physics::PhysicsSystem::simulatePhysics(std::pair<const Entity::EntityID, Physics::PhysicComponent>& physicComp, 
//                                                        const Physics::PhysicsSystem::PhysicsAdditionalData& data,
//                                                        COLLISIONS_CALLBACKS& callbacks)
// {
//     if (!physicComp.second.isEnabled)
//         return;

//     SegmentHit hit;
//     hit.t = 2.f;

//     Entity::EntityID collidedEntity; // box

//     if (staticBoxesFirstCollision(physicComp.second, physicComp.second.collider.transform->transform.location, hit, data, collidedEntity))
//     {
//         Core::Maths::Vec3 newLoc = collisionPhysicalResponse(physicComp.second, physicComp.second.collider.transform->transform.location, hit);

//         staticBoxesOverlapCollision(physicComp.second, physicComp.second.collider.transform->transform.location, newLoc, data, physicComp.first, callbacks);

//         if (!physicComp.second.collider.isColliding)
//         {
//             physicComp.second.collider.isColliding = true;

//             // Callback
//             CollisionsCallbacksSentData collisionsCallbacksSentData
//             {
//                 std::move(hit),
//                 physicComp.first,
//                 collidedEntity
//             };
//             callbacks.onCollisionEnter(collisionsCallbacksSentData);
//         }

//         physicComp.second.collider.transform->transform.location = newLoc;
//         return;
//     }
    
//     // is not colliding
//     else if (physicComp.second.collider.isColliding)
//     {
//         physicComp.second.collider.isColliding = false;

//         // Callback
//         callbacks.onCollisionExit(physicComp.first);
//     }

//     staticBoxesOverlapCollision(physicComp.second, physicComp.second.collider.transform->transform.location, 
//                                 physicComp.second.velocity + physicComp.second.collider.transform->transform.location, 
//                                 data, 
//                                 physicComp.first, 
//                                 callbacks);

//     physicComp.second.collider.transform->transform.location = physicComp.second.collider.transform->transform.location + physicComp.second.velocity;
//     return;
// }

template<typename COLLISIONS_CALLBACKS>
void Physics::PhysicsSystem::staticBoxesOverlapCollision(Physics::PhysicComponent& physicComp, 
                                                         const Core::Maths::Vec3& startLoc, 
                                                         const Core::Maths::Vec3& endLoc, 
                                                         const PhysicsAdditionalData& data,
                                                         const Entity::EntityID& physicCompEntityID,
                                                         COLLISIONS_CALLBACKS& callbacks)
{   
    Segment3D seg{startLoc, endLoc};
    if (seg.squaredLength() < 0.0001)
        return;
    SegmentHit hit;
    
    // // TODO : opti with reserve
    // std::vector<CollisionsCallbacksSentDataCpy> collisionsCallbacksSentDataList;
    // collisionsCallbacksSentDataList.reserve(nextNbOverlapHint); 

    for (std::pair<const Entity::EntityID, Physics::CollisionComponent<Box>>& boxCollider : boxes)
    {
        if (!boxCollider.second.isEnabled || !boxCollider.second.isOverlap || data.ignoredEntities.count(boxCollider.first) > 0)
            continue;

        boxCollider.second.worldCollider.updateMatrixSizeFromMatrix();

        if (Collisions::boxMovingShereCollision(boxCollider.second.worldCollider, physicComp.collider.worldCollider, seg, hit))
        {
            // // Add Callbacks to list
            // collisionsCallbacksSentDataList.emplace_back(hit, physicCompEntityID, boxCollider.first);
            callbacks.onOverlap(CollisionsCallbacksSentData{hit, physicCompEntityID, boxCollider.first});
        }
    }
}