#include "physicsSystem.hpp"

#include "physicComponent.hpp"
#include "collisionComponent.hpp"
#include "physicComponentInterface.hpp"
#include "collisionComponentInterface.hpp"
#include "basicEntity.hpp"
#include "collisions.hpp"
#include "engine.hpp"
#include "camera.hpp"

// template<typename T = Box>
template<>
Physics::PhysicsSystem::ColliderIt<Box> Physics::PhysicsSystem::addCollider<Box>(Physics::CollisionComponentInterface<Box>* collider)
{
    // boxes.emplace(entity, Physics::CollisionComponent<Box>());
    // return Physics::PhysicsSystem::ColliderIt<Box>{entity, &boxes};

    boxes.emplace_back(collider);
    return Physics::PhysicsSystem::ColliderIt<Box>{boxes.size() - 1};
}

// template<> 
// void Physics::PhysicsSystem::loadColliderItContainer<Box>(ColliderIt<Box>& it, const Entity::EntityID& entityID)
// {
//     it.entityID = entityID;
//     it.mapPtr = &boxes;
// }


void Physics::PhysicsSystem::simulateGravity(Physics::PhysicComponent& physicComp, const Core::Engine& engine)
{
    physicComp.velocity.y -= gravityAcc * engine.deltaTime;
}

// Core::Maths::Vec3 Physics::PhysicsSystem::collisionPhysicalResponse(Physics::PhysicComponent& physicComp, 
//                                                            const Core::Maths::Vec3& startLoc, 
//                                                            SegmentHit& hit)
// {
//     Core::Maths::Vec3 newLoc = physicComp.velocity * hit.t + startLoc;

//     Core::Maths::Vec3 velocityAfterContact = physicComp.velocity * (1.f - hit.t);

//     // hit.normal = hit.normal.unitVector();
//     float dot = Core::Maths::Vec3::dotProduct(hit.normal, velocityAfterContact);

//     Core::Maths::Vec3 finalLoc = startLoc + physicComp.velocity - dot * hit.normal;
//     physicComp.velocity = (physicComp.velocity - dot * hit.normal);

//     // We add a small value, because otherwises, they player would not be able to move due to constant collisions
//     return finalLoc + hit.normal * minimalDistToGround;
// }


bool Physics::PhysicsSystem::staticBoxesFirstCollision(Physics::PhysicComponent& physicComp, const Core::Maths::Vec3& startLoc, 
                                                       SegmentHit& hit, const PhysicsAdditionalData& data, 
                                                       Physics::CollisionComponentInterface<Box>*& collidedEntityID,
                                                       const Core::Maths::Vec3& velocity)
{
    bool hasCollided = false;
    SegmentHit segmentHit;
    
    Segment3D seg{startLoc, startLoc + velocity};

    for (Physics::CollisionComponentInterface<Box>* boxCollider : boxes)
    {
        if (!boxCollider->collider.isEnabled || boxCollider->collider.isOverlap || data.ignoredEntities.count(boxCollider) > 0)
            continue;

        // boxCollider.second.worldCollider.transform = boxCollider.second.transform->transformMatrixNode->worldData; 
        //                                             // * physicComp.collider.transform->transformMatrixNode->worldData.getInverse();

        boxCollider->collider.worldCollider.updateMatrixSizeFromMatrix();

        if (Collisions::boxMovingShereCollision(boxCollider->collider.worldCollider, physicComp.collider.worldCollider, seg, segmentHit))
        {
            if (segmentHit.t <= hit.t)
            {
                hit = segmentHit;

                hasCollided = true;
                collidedEntityID = boxCollider;
            }
        }
    }

    return hasCollided;
}


bool Physics::PhysicsSystem::isSegmentColliding(Renderer::Camera& camera, const Core::Maths::Vec3& forward)
{
    for (Physics::CollisionComponentInterface<Box>* boxCollider : boxes)
    {
        SegmentHit hit;
        Segment3D seg;
        seg.p1 = {0.f, 0, 0};
        seg.p2 = {0.f, 0, -10};
        seg.p1 = camera.transform.transformMatrixNode->worldData * seg.p1;
        seg.p2 = camera.transform.transformMatrixNode->worldData * seg.p2;

        if (Collisions::boxSegmentCollision(boxCollider->collider.worldCollider, seg, hit))
        {
            return true;
        }
    }
    return false;
}

bool Physics::PhysicsSystem::raycast(const Segment3D& seg, SegmentHit& hit, Physics::CollisionComponentInterface<Box>*& touchedEntity) const
{
    hit.t = 2.f;

    for (Physics::CollisionComponentInterface<Box>* boxCollider : boxes)
    {

        SegmentHit tempHit;
        if (Collisions::boxSegmentCollision(boxCollider->collider.worldCollider, seg, tempHit))
        {
            if (tempHit.t < hit.t)
            {
                touchedEntity = boxCollider;
                hit = tempHit;
            }
        }
    }

    return hit.t >= 0.f && hit.t <= 1.f;
}

void Physics::PhysicsSystem::reset()
{
    boxes.clear();
    physicComponents.clear();
}
















inline Physics::PhysicsSystem::PhysicCompIt Physics::PhysicsSystem::addPhysicComponent(Physics::PhysicComponentInterface* physicComp)
{
    // physicComponents.emplace(entity, Physics::PhysicComponent());
    // return Physics::PhysicsSystem::PhysicCompIt{entity, &physicComponents};
    physicComponents.emplace_back(physicComp);
    return Physics::PhysicsSystem::PhysicCompIt{physicComponents.size() - 1};
}


void Physics::PhysicsSystem::simulate(Core::Engine& engine)
{
    // for (std::pair<const Entity::EntityID, Physics::PhysicComponent>& physicComp : physicComponents)
    for (Physics::PhysicComponentInterface* physicComp : physicComponents)
    {
        physicComp->physicComp.collider.worldCollider.center = physicComp->physicComp.collider.transform->transform.location;
        
        // for (std::pair<const Entity::EntityID, Physics::CollisionComponent<Box>>& box : boxes)
        for (Physics::CollisionComponentInterface<Box>* box : boxes)
        {
            box->collider.worldCollider.transform = box->collider.transform->transformMatrixNode->worldData;
                                            //    * physicComp->collider.transform->transformMatrixNode->worldData.getInverse();
        }

        if (physicComp->physicComp.isEnabled)
        {
            Physics::PhysicsSystem::PhysicsAdditionalData playerIgnoreData;
            playerIgnoreData.ignoredEntities.emplace(physicComp);
            // simulatePhysics(physicComp, 
            //                 playerIgnoreData, 
            //                 physicComp->velocity,
            //                 callbacks);
            Sphere sphere;
            sphere.center = physicComp->physicComp.collider.worldCollider.center;
            sphere.radius = physicComp->physicComp.collider.worldCollider.radius; 
        // if (leftVelocity.y > 0)
        // std::cout << leftVelocity << std::endl;

            std::map<Entity::EntityID, bool>::iterator it = physicComp->physicComp.collider.collidingEntities.begin();
            while (it != physicComp->physicComp.collider.collidingEntities.end())
            {
                it->second = false;
                ++it;
            }

            Core::Maths::Vec3 temp = physicComp->physicComp.collider.transform->transform.location + physicComp->physicComp.velocity; 
            Core::Maths::Vec3& leftVelocity = physicComp->physicComp.velocity;
            physicComp->physicComp.collider.transform->transform.location = simulatePhysicsForASphere(sphere, 
                                                                                            playerIgnoreData,  
                                                                                            physicComp->physicComp.collider.collidingEntities,
                                                                                            leftVelocity,
                                                                                            physicComp);

            std::map<Entity::EntityID, bool>::iterator itColliding = physicComp->physicComp.collider.collidingEntities.begin();
            while (itColliding != physicComp->physicComp.collider.collidingEntities.end())
            {
                if (!itColliding->second)
                {
                    // callbacks.onCollisionExit(physicComp);//it->first);
                    physicComp->physicCompOnCollisionExit();  
                    itColliding = physicComp->physicComp.collider.collidingEntities.erase(itColliding);
                }
                else 
                    ++itColliding;
            }
            // bool bCollides = temp != physicComp.second.collider.transform->transform.location;

            // if (physicComp.second.collider.isColliding)
            //     physicComp.second.velocity *= linearDamping;

            simulateGravity(physicComp->physicComp, engine);

            physicComp->physicComp.collider.transform->UpdateLocalTransformMatrix();
            physicComp->physicComp.collider.transform->transformMatrixNode->cleanUpdate();
        }
    }
}

bool Physics::PhysicsSystem::sphereCollisionWithBoxes(const Sphere& sphere, 
                                                      const Core::Maths::Vec3& velocity,
                                                      const Physics::PhysicsSystem::PhysicsAdditionalData& data, 
                                                      SegmentHit& hit,
                                                      Physics::CollisionComponentInterface<Box>*& collidedMeshInterface)
{
    bool hasCollided = false;
    SegmentHit segmentHit;
    
    Segment3D seg{sphere.center, sphere.center + velocity};

    // for (std::pair<const Entity::EntityID, Physics::CollisionComponent<Box>>& boxCollider : boxes)
    for (Physics::CollisionComponentInterface<Box>* boxCollider : boxes)
    {
        if (!boxCollider->collider.isEnabled || boxCollider->collider.isOverlap)// || data.ignoredEntities.count(boxCollider.first) > 0) // TODO : ignored entities
            continue;

        // boxCollider->collider.worldCollider.transform = boxCollider->collider.transform->transformMatrixNode->worldData; 
        //                                             // * physicComp.collider.transform->transformMatrixNode->worldData.getInverse();

        boxCollider->collider.worldCollider.updateMatrixSizeFromMatrix();

        if (Collisions::boxMovingShereCollision(boxCollider->collider.worldCollider, sphere, seg, segmentHit))
        {
            if (segmentHit.t <= hit.t)
            {
                hit = segmentHit;

                hasCollided = true;
                collidedMeshInterface = boxCollider;
            }
        }
    }

    return hasCollided;
}

void Physics::PhysicsSystem::sphereFindOverlappingBoxes(const Sphere& sphere, 
                                                      const Core::Maths::Vec3& lastLoc,
                                                      const Physics::PhysicsSystem::PhysicsAdditionalData& data,
                                                      const Physics::PhysicComponentInterface* physicCompID)
{
    Segment3D seg{sphere.center, lastLoc};
    if (seg.squaredLength() < 0.0001)
        return;

    SegmentHit hit;
    
    // // TODO : opti with reserve
    // std::vector<CollisionsCallbacksSentDataCpy> collisionsCallbacksSentDataList;
    // collisionsCallbacksSentDataList.reserve(nextNbOverlapHint); 

    // for (std::pair<const Entity::EntityID, Physics::CollisionComponent<Box>>& boxCollider : boxes)
    for (Physics::CollisionComponentInterface<Box>* boxCollider : boxes)
    {
        if (!boxCollider->collider.isEnabled || !boxCollider->collider.isOverlap || data.ignoredEntities.count(boxCollider) > 0)
            continue;

        boxCollider->collider.worldCollider.updateMatrixSizeFromMatrix();

        if (Collisions::boxMovingShereCollision(boxCollider->collider.worldCollider, sphere, seg, hit))
        {
            // // Add Callbacks to list
            // collisionsCallbacksSentDataList.emplace_back(hit, physicCompEntityID, boxCollider.first);
            // callbacks.onOverlap(CollisionsCallbacksSentData{hit, physicCompID, boxCollider.first});
            boxCollider->colliderOnOverlapEnter(CollisionsCallbacksSentData{hit, physicCompID, boxCollider});
        }
    }
}

inline Core::Maths::Vec3 Physics::PhysicsSystem::simulatePhysicsForASphere(const Sphere& sphere, 
                               const Physics::PhysicsSystem::PhysicsAdditionalData& data, 
                               std::map<Entity::EntityID, bool>& collidingEntities,
                               Core::Maths::Vec3& leftVelocity,
                               Physics::PhysicComponentInterface* physicCompID)
{
    if (leftVelocity.vectorSquareLength() < 0.00001f)
    {
        return sphere.center;
    }

    SegmentHit hit;
    hit.t = 2.f;
    // Entity::EntityID collidedEntity; // box
    Physics::CollisionComponentInterface<Box>* collidedMeshInterface; // box

    if (sphereCollisionWithBoxes(sphere, leftVelocity, data, hit, collidedMeshInterface))
    {
        std::pair<std::map<Entity::EntityID, bool>::iterator, bool> it = collidingEntities.emplace(collidedMeshInterface, true);
        if (!it.second)
        {
            // Callback
            CollisionsCallbacksSentData collisionsCallbacksSentData
            {
                std::move(hit),
                physicCompID,
                collidedMeshInterface
            };
            physicCompID->physicCompOnCollisionEnter(hit);
            collidedMeshInterface->colliderOnCollisionEnter(hit);
            // callbacks.onCollisionEnter(collisionsCallbacksSentData);
        }
         


        Core::Maths::Vec3 newLoc = sphere.center + leftVelocity * hit.t;
        Core::Maths::Vec3 newVelo = leftVelocity * hit.t;
        Core::Maths::Vec3 velocityAfterContact = leftVelocity - newVelo;
        float dot = Core::Maths::Vec3::dotProduct(hit.normal, velocityAfterContact);
        Core::Maths::Vec3 finalLoc = sphere.center + velocityAfterContact - dot * hit.normal;// + 0.001f * hit.normal;
        leftVelocity = (leftVelocity - dot * hit.normal);
        leftVelocity *= 0.9f;

        sphereFindOverlappingBoxes(sphere, finalLoc, data, physicCompID);

        
        // std::cout << "left : " << physicComp.second.velocity << std::endl;

        // // We add a small value, because otherwises, they player would not be able to move due to constant collisions
        // return finalLoc + hit.normal * minimalDistToGround;

        Sphere s;
        s.center = finalLoc;
        s.radius= sphere.radius;

        return simulatePhysicsForASphere(sphere, data, collidingEntities, leftVelocity, physicCompID);
    }
    else 
    {
        Core::Maths::Vec3 finalLoc = sphere.center + leftVelocity; 
        sphereFindOverlappingBoxes(sphere, finalLoc, data, physicCompID);
        return finalLoc;
    }
}

void Physics::PhysicsSystem::simulatePhysics(Physics::PhysicComponentInterface* physicComp, 
                                            const Physics::PhysicsSystem::PhysicsAdditionalData& data,
                                            const Core::Maths::Vec3& leftVelocity)
{
    if (leftVelocity.vectorSquareLength() < 0.001f)
        return;

    // std::cout << leftVelocity << std::endl;
    SegmentHit hit;
    hit.t = 2.f;
    // Entity::EntityID collidedEntity; // box
    Physics::CollisionComponentInterface<Box>* collidedEntity = nullptr; // box

    if (staticBoxesFirstCollision(physicComp->physicComp, physicComp->physicComp.collider.transform->transform.location, hit, data, collidedEntity, leftVelocity))
    {
        Core::Maths::Vec3 newLoc = physicComp->physicComp.collider.transform->transform.location + leftVelocity * hit.t;
        // std::cout << "Old : " << physicComp.second.collider.transform->transform.location << '\t' << " New : " << newLoc << std::endl; 
        // std::cout << "Hit.t : " << hit.t << std::endl;
        staticBoxesOverlapCollision(physicComp->physicComp, 
                                    physicComp->physicComp.collider.transform->transform.location, 
                                    newLoc, 
                                    data);

        physicComp->physicComp.collider.transform->transform.location = newLoc;

        if (!physicComp->physicComp.collider.isColliding)
        {
            physicComp->physicComp.collider.isColliding = true;

            // Callback
            CollisionsCallbacksSentData collisionsCallbacksSentData
            {
                std::move(hit),
                physicComp,
                collidedEntity
            };
            physicComp->physicCompOnCollisionEnter(hit);
            collidedEntity->colliderOnCollisionEnter(hit);
            // callbacks.onCollisionEnter(collisionsCallbacksSentData);
        }

        Core::Maths::Vec3 newVelo = leftVelocity * hit.t;

        Core::Maths::Vec3 velocityAfterContact = leftVelocity - newVelo;

        // hit.normal = hit.normal.unitVector();
        float dot = Core::Maths::Vec3::dotProduct(hit.normal, velocityAfterContact);

        Core::Maths::Vec3 finalLoc = physicComp->physicComp.collider.transform->transform.location + velocityAfterContact - dot * hit.normal + 0.001f * hit.normal;
        physicComp->physicComp.velocity = (leftVelocity - dot * hit.normal);
        // std::cout << "left : " << physicComp.second.velocity << std::endl;

        // // We add a small value, because otherwises, they player would not be able to move due to constant collisions
        // return finalLoc + hit.normal * minimalDistToGround;
        simulatePhysics(physicComp, data, physicComp->physicComp.velocity);
    }
    else 
    {
        if (physicComp->physicComp.collider.isColliding)
        {
            physicComp->physicComp.collider.isColliding = false;

            // Callback
            // callbacks.onCollisionExit(physicComp.first);
            physicComp->physicCompOnCollisionExit();
        }

        physicComp->physicComp.collider.transform->transform.location = physicComp->physicComp.collider.transform->transform.location + leftVelocity;
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

void Physics::PhysicsSystem::staticBoxesOverlapCollision(Physics::PhysicComponent& physicComp, 
                                                         const Core::Maths::Vec3& startLoc, 
                                                         const Core::Maths::Vec3& endLoc, 
                                                         const PhysicsAdditionalData& data,
                                                         const Entity::EntityID& physicCompEntityID)
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
        if (!boxCollider.second.isEnabled || !boxCollider.second.isOverlap || data.ignoredEntities.count(boxCollider) > 0)
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






