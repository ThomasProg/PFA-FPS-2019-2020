#include "physicsSystem.hpp"

#include "gTransform.hpp"
#include "engine.hpp"
#include <vector>

// template<typename T = Sphere>
inline Physics::PhysicsSystem::PhysicCompIt Physics::PhysicsSystem::addPhysicComponent(Physics::PhysicComponent* physicComp)
{
    // physicComponents.emplace(entity, Physics::PhysicComponent());
    // return Physics::PhysicsSystem::PhysicCompIt{entity, &physicComponents};
    physicComponents.emplace_back(physicComp);
    return Physics::PhysicsSystem::PhysicCompIt{physicComponents.size() - 1};
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
            // simulatePhysics(physicComp, 
            //                 playerIgnoreData, 
            //                 physicComp.second.velocity,
            //                 callbacks);
            Sphere sphere;
            sphere.center = physicComp.second.collider.worldCollider.center;
            sphere.radius = physicComp.second.collider.worldCollider.radius; 
        // if (leftVelocity.y > 0)
        // std::cout << leftVelocity << std::endl;

            std::map<Entity::EntityID, bool>::iterator it = physicComp.second.collider.collidingEntities.begin();
            while (it != physicComp.second.collider.collidingEntities.end())
            {
                it->second = false;
                ++it;
            }

            Core::Maths::Vec3 temp = physicComp.second.collider.transform->transform.location + physicComp.second.velocity; 
            Core::Maths::Vec3& leftVelocity = physicComp.second.velocity;
            physicComp.second.collider.transform->transform.location = simulatePhysicsForASphere(sphere, 
                                                                                                playerIgnoreData,  
                                                                                                physicComp.second.collider.collidingEntities,
                                                                                                leftVelocity,
                                                                                                physicComp.first,
                                                                                                callbacks);

            std::map<Entity::EntityID, bool>::iterator itColliding = physicComp.second.collider.collidingEntities.begin();
            while (itColliding != physicComp.second.collider.collidingEntities.end())
            {
                if (!itColliding->second)
                {
                    callbacks.onCollisionExit(physicComp.first);//it->first);  
                    itColliding = physicComp.second.collider.collidingEntities.erase(itColliding);
                }
                else 
                    ++itColliding;
            }
            // bool bCollides = temp != physicComp.second.collider.transform->transform.location;

            // if (physicComp.second.collider.isColliding)
            //     physicComp.second.velocity *= linearDamping;

            simulateGravity(physicComp.second, engine);

            physicComp.second.collider.transform->UpdateLocalTransformMatrix();
            physicComp.second.collider.transform->transformMatrixNode->cleanUpdate();
        }
    }
}

bool Physics::PhysicsSystem::sphereCollisionWithBoxes(const Sphere& sphere, 
                                                      const Core::Maths::Vec3& velocity,
                                                      const Physics::PhysicsSystem::PhysicsAdditionalData& data, 
                                                      SegmentHit& hit,
                                                      Entity::EntityID& collidedEntityID)
{
    bool hasCollided = false;
    SegmentHit segmentHit;
    
    Segment3D seg{sphere.center, sphere.center + velocity};

    // for (std::pair<const Entity::EntityID, Physics::CollisionComponent<Box>>& boxCollider : boxes)
    for (Physics::CollisionComponentInterface<Box>>* boxCollider : boxes)
    {
        if (!boxCollider.second.isEnabled || boxCollider.second.isOverlap || data.ignoredEntities.count(boxCollider.first) > 0)
            continue;

        // boxCollider.second.worldCollider.transform = boxCollider.second.transform->transformMatrixNode->worldData; 
        //                                             // * physicComp.collider.transform->transformMatrixNode->worldData.getInverse();

        boxCollider.second.worldCollider.updateMatrixSizeFromMatrix();

        if (Collisions::boxMovingShereCollision(boxCollider.second.worldCollider, sphere, seg, segmentHit))
        {
            if (segmentHit.t <= hit.t)
            {
                hit = segmentHit;

                hasCollided = true;
                collidedEntityID = boxCollider.first;
            }
        }
    }

    return hasCollided;
}

template<typename COLLISIONS_CALLBACKS>
void Physics::PhysicsSystem::sphereFindOverlappingBoxes(const Sphere& sphere, 
                                                      const Core::Maths::Vec3& lastLoc,
                                                      const Physics::PhysicsSystem::PhysicsAdditionalData& data,
                                                      const Entity::EntityID& physicCompID,
                                                      COLLISIONS_CALLBACKS& callbacks)
{
    Segment3D seg{sphere.center, lastLoc};
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

        if (Collisions::boxMovingShereCollision(boxCollider.second.worldCollider, sphere, seg, hit))
        {
            // // Add Callbacks to list
            // collisionsCallbacksSentDataList.emplace_back(hit, physicCompEntityID, boxCollider.first);
            callbacks.onOverlap(CollisionsCallbacksSentData{hit, physicCompID, boxCollider.first});
        }
    }
}

template<typename COLLISIONS_CALLBACKS>
inline Core::Maths::Vec3 Physics::PhysicsSystem::simulatePhysicsForASphere(const Sphere& sphere, 
                               const Physics::PhysicsSystem::PhysicsAdditionalData& data, 
                               std::map<Entity::EntityID, bool>& collidingEntities,
                               Core::Maths::Vec3& leftVelocity,
                               const Entity::EntityID& physicCompID,
                               COLLISIONS_CALLBACKS& callbacks)
{
    if (leftVelocity.vectorSquareLength() < 0.00001f)
    {
        return sphere.center;
    }

    SegmentHit hit;
    hit.t = 2.f;
    Entity::EntityID collidedEntity; // box

    if (sphereCollisionWithBoxes(sphere, leftVelocity, data, hit, collidedEntity))
    {
        std::pair<std::map<Entity::EntityID, bool>::iterator, bool> it = collidingEntities.emplace(collidedEntity, true);
        if (!it.second)
        {
            // Callback
            CollisionsCallbacksSentData collisionsCallbacksSentData
            {
                std::move(hit),
                physicCompID,
                collidedEntity
            };
            callbacks.onCollisionEnter(collisionsCallbacksSentData);
        }
         


        Core::Maths::Vec3 newLoc = sphere.center + leftVelocity * hit.t;
        Core::Maths::Vec3 newVelo = leftVelocity * hit.t;
        Core::Maths::Vec3 velocityAfterContact = leftVelocity - newVelo;
        float dot = Core::Maths::Vec3::dotProduct(hit.normal, velocityAfterContact);
        Core::Maths::Vec3 finalLoc = sphere.center + velocityAfterContact - dot * hit.normal;// + 0.001f * hit.normal;
        leftVelocity = (leftVelocity - dot * hit.normal);
        leftVelocity *= 0.9f;

        sphereFindOverlappingBoxes(sphere, finalLoc, data, physicCompID, callbacks);

        
        // std::cout << "left : " << physicComp.second.velocity << std::endl;

        // // We add a small value, because otherwises, they player would not be able to move due to constant collisions
        // return finalLoc + hit.normal * minimalDistToGround;

        Sphere s;
        s.center = finalLoc;
        s.radius= sphere.radius;

        return simulatePhysicsForASphere(sphere, data, collidingEntities, leftVelocity, physicCompID, callbacks);
    }
    else 
    {
        Core::Maths::Vec3 finalLoc = sphere.center + leftVelocity; 
        sphereFindOverlappingBoxes(sphere, finalLoc, data, physicCompID, callbacks);
        return finalLoc;
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