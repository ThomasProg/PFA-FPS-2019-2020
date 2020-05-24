#include "physicsSystem.hpp"

#include "physicComponent.hpp"
#include "collisionComponent.hpp"
#include "physicComponentInterface.hpp"
#include "collisionComponentInterface.hpp"
#include "basicEntity.hpp"
#include "collisions.hpp"
#include "engine.hpp"
#include "camera.hpp"

template<>
Physics::PhysicsSystem::ColliderIt<Box> Physics::PhysicsSystem::addCollider<Box>(Physics::CollisionComponentInterface<Box>* collider)
{
    boxes.emplace_back(collider);
    return Physics::PhysicsSystem::ColliderIt<Box>{boxes.size() - 1};
}

Physics::PhysicsSystem::PhysicCompIt Physics::PhysicsSystem::addPhysicComponent(Physics::PhysicComponentInterface* physicComp)
{
    physicComponents.emplace_back(physicComp);
    return Physics::PhysicsSystem::PhysicCompIt{physicComponents.size() - 1};
}

void Physics::PhysicsSystem::simulateGravity(Physics::PhysicComponent& physicComp, const Core::Engine& engine)
{
    physicComp.velocity.y -= gravityAcc * engine.deltaTime * engine.deltaTime;
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


void Physics::PhysicsSystem::simulatePhysics(Core::Engine& engine)
{
    // Update boxes transform
    for (Physics::CollisionComponentInterface<Box>* box : boxes)
    {
        box->collider.worldCollider.transform = box->collider.transform->transformMatrixNode->worldData;
        box->collider.aabb.setFrom(box->collider.worldCollider);
    }

    for (Physics::PhysicComponentInterface* physicComp : physicComponents)
    {
        if (!physicComp->physicComp.isEnabled)
            continue;
        
        physicComp->physicComp.collider.aabb.setFrom(physicComp->physicComp.collider.worldCollider);
        simulatePhysicsForPhysicComp(physicComp, engine);
    }
}

void Physics::PhysicsSystem::simulatePhysicsForPhysicComp(Physics::PhysicComponentInterface* physicComp, Core::Engine& engine)
{
    assert(physicComp != nullptr);

    // Resets colliding entities
    std::map<CollisionComponentInterface<Box>*, bool>::iterator it = physicComp->physicComp.collider.collidingEntities.begin();
    while (it != physicComp->physicComp.collider.collidingEntities.end())
    {
        it->second = false;
        ++it;
    }

    simulatePhysicsForASphere(physicComp, engine);

    // Sets colliding entities
    std::map<CollisionComponentInterface<Box>*, bool>::iterator itColliding = physicComp->physicComp.collider.collidingEntities.begin();
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

    // if (physicComp.second.collider.isColliding)
    //     physicComp.second.velocity *= linearDamping;

    simulateGravity(physicComp->physicComp, engine);

    physicComp->physicComp.collider.transform->UpdateLocalTransformMatrix();
    physicComp->physicComp.collider.transform->transformMatrixNode->cleanUpdate();
}

void Physics::PhysicsSystem::simulatePhysicsForASphere(Physics::PhysicComponentInterface* physicComp, Core::Engine& engine)
{
    assert(physicComp != nullptr);
    
    // Update Sphere properties
    physicComp->physicComp.collider.worldCollider.center = physicComp->physicComp.collider.transform->transform.location;

    Physics::PhysicsSystem::PhysicsAdditionalData playerIgnoreData;
    playerIgnoreData.ignoredEntities.emplace(nullptr);
    physicComp->physicComp.collider.transform->transform.location = simulateCollisionsForASphere( 
                                                                                    playerIgnoreData,  
                                                                                    physicComp);
}

bool Physics::PhysicsSystem::sphereCollisionWithBoxes(const Sphere& sphere, 
                                                      const Core::Maths::Vec3& velocity,
                                                      const Physics::PhysicsSystem::PhysicsAdditionalData& data, 
                                                      SegmentHit& hit,
                                                      Physics::CollisionComponentInterface<Box>*& collidedMeshInterface)
{
    bool hasCollided = false;
    SegmentHit segmentHit;

    // TODO : Remove and replace args
    AABB aabbFromSphere;
    aabbFromSphere.setFrom(sphere);  

    Sphere s2 = sphere;
    s2.center += velocity;
    AABB aabbFromSphere2;
    aabbFromSphere2.setFrom(s2);  

    AABB totalAabb = aabbFromSphere + aabbFromSphere2;
    //////////////////////////////////  

    Segment3D seg{sphere.center, sphere.center + velocity};

    for (Physics::CollisionComponentInterface<Box>* boxCollider : boxes)
    {
        if (!boxCollider->collider.isEnabled || boxCollider->collider.isOverlap)// || data.ignoredEntities.count(boxCollider.first) > 0) // TODO : ignored entities
            continue;

        if (!Collisions::aabbAabbCollision(boxCollider->collider.aabb, totalAabb))
            continue;

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
                                                      Physics::PhysicComponentInterface* physicComp)
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

        if (Collisions::boxMovingShereCollision(boxCollider->collider.worldCollider, sphere, seg, hit))
        {
            // // Add Callbacks to list
            // collisionsCallbacksSentDataList.emplace_back(hit, physicCompEntityID, boxCollider.first);
            // callbacks.onOverlap(CollisionsCallbacksSentData{hit, physicComp, boxCollider.first});
            CollisionsCallbacksSentData data {hit, physicComp, boxCollider};
            boxCollider->colliderOnOverlapEnter(data);
            physicComp->physicCompOnOverlapEnter(data);
        }
    }
}

Core::Maths::Vec3 Physics::PhysicsSystem::simulateCollisionsForASphere( 
                               const Physics::PhysicsSystem::PhysicsAdditionalData& data, 
                               Physics::PhysicComponentInterface* physicComp)
{
    Sphere& sphere = physicComp->physicComp.collider.worldCollider;
    std::map<Physics::CollisionComponentInterface<Box>*, bool>& collidingEntities = physicComp->physicComp.collider.collidingEntities;
    Core::Maths::Vec3& leftVelocity = physicComp->physicComp.velocity;

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
        std::pair<std::map<CollisionComponentInterface<Box>*, bool>::iterator, bool> it = collidingEntities.emplace(collidedMeshInterface, true);
        if (!it.second)
        {
            physicComp->physicCompOnCollisionEnter(hit);
            collidedMeshInterface->colliderOnCollisionEnter(hit);
        }

        Core::Maths::Vec3 velocityAfterContact = leftVelocity * (1.f - hit.t);
        float dot = Core::Maths::Vec3::dotProduct(hit.normal, velocityAfterContact);
        Core::Maths::Vec3 finalLoc = sphere.center + velocityAfterContact - dot * hit.normal;
        leftVelocity -= dot * hit.normal;
        leftVelocity *= 0.9f;

        sphereFindOverlappingBoxes(sphere, finalLoc, data, physicComp);

        sphere.center = finalLoc;

        return simulateCollisionsForASphere(data, physicComp);
    }
    else 
    {
        Core::Maths::Vec3 finalLoc = sphere.center + leftVelocity;
        sphereFindOverlappingBoxes(sphere, finalLoc, data, physicComp);
        return finalLoc;
    }
}





