#include "physicsSystem.hpp"

#include "physicComponent.hpp"
#include "collisionComponent.hpp"
#include "physicComponentInterface.hpp"
#include "collisionComponentInterface.hpp"
#include "collisions.hpp"
#include "engine.hpp"
#include "camera.hpp"

template<>
Physics::PhysicsSystem::ColliderIt<Physics::Shapes::Box> 
    Physics::PhysicsSystem::addCollider<Physics::Shapes::Box>(Physics::CollisionComponentInterface<Physics::Shapes::Box>* collider)
{
    assert(collider != nullptr);
    if (freeBoxesIndices.empty())
    {
        boxes.emplace_back(collider);
        return Physics::PhysicsSystem::ColliderIt<Physics::Shapes::Box>{((unsigned int) boxes.size()) - 1u};
    }
    else 
    {
        Physics::PhysicsSystem::ColliderIt<Physics::Shapes::Box> newIt = freeBoxesIndices.back(); 
        boxes[newIt.arrayIndex] = collider;
        // Always constant, compared to erasing the first element.
        freeBoxesIndices.pop_back(); 
        return newIt;
    }
}

Physics::PhysicsSystem::PhysicCompIt Physics::PhysicsSystem::addPhysicComponent(Physics::PhysicComponentInterface* physicComp)
{
    assert(physicComp != nullptr);
    if (freePhysicCompsIndices.empty())
    {
        physicComponents.emplace_back(physicComp);
        return Physics::PhysicsSystem::PhysicCompIt{((unsigned int) physicComponents.size()) - 1u};
    }
    else 
    {
        Physics::PhysicsSystem::PhysicCompIt newIt = freePhysicCompsIndices.back(); 
        physicComponents[newIt.arrayIndex] = physicComp;
        // Always constant, compared to erasing another element.
        freePhysicCompsIndices.pop_back(); 
        return newIt;
    }
}

void Physics::PhysicsSystem::erase(PhysicCompIt& it)
{
    // We check if the index is valid (inside the container).
    // It it valid, that means there is a last element, 
    // that we can get with back(), and that we can remove with pop_back().
    assert(it.arrayIndex < physicComponents.size());
    physicComponents[it.arrayIndex] = nullptr;
    freePhysicCompsIndices.emplace_back(it);
}

void Physics::PhysicsSystem::erase(ColliderIt<Physics::Shapes::Box>& it)
{
    // We check if the index is valid (inside the container).
    // It it valid, that means there is a last element, 
    // that we can get with back(), and that we can remove with pop_back().
    assert(it.arrayIndex < boxes.size());
    boxes[it.arrayIndex] = nullptr;
    freeBoxesIndices.emplace_back(it);
}

void Physics::PhysicsSystem::simulateGravity(Physics::PhysicComponent& physicComp, const Core::Engine& engine)
{
    // m / s                 (N / kg) * s   = kg * m * s^(-2) * s = m / s
    physicComp.velocity.y -= (gravityAcc *  physicComp.mass) * engine.deltaTime;
}

bool Physics::PhysicsSystem::raycast(const Physics::Shapes::Segment3D& seg, Physics::Shapes::SegmentHit& hit, 
                                     Physics::CollisionComponentInterface<Physics::Shapes::Box>*& touchedEntity,
                                     unsigned int consideredLayers) const
{
    hit.t = 2.f;

    for (Physics::CollisionComponentInterface<Physics::Shapes::Box>* boxCollider : boxes)
    {
        if (boxCollider == nullptr || ((consideredLayers & boxCollider->collider.layers) == 0))
            continue;

        Physics::Shapes::SegmentHit tempHit;
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
    freeBoxesIndices.clear();
    freePhysicCompsIndices.clear();
}


void Physics::PhysicsSystem::simulatePhysics(Core::Engine& engine)
{
    // Update boxes transform
    for (Physics::CollisionComponentInterface<Physics::Shapes::Box>* box : boxes)
    {
        if (box == nullptr)
            continue;

        box->collider.worldCollider.transform = box->collider.transform->transformMatrixNode->getWorldMatrix();
        box->collider.aabb.setFrom(box->collider.worldCollider);
    }

    // Simulate physics for all physic components
    for (Physics::PhysicComponentInterface* physicComp : physicComponents)
    {
        if (physicComp == nullptr || !physicComp->physicComp.isEnabled)
            continue;
        
        physicComp->physicComp.collider.aabb.setFrom(physicComp->physicComp.collider.worldCollider);
        simulatePhysicsForPhysicComp(physicComp, engine);
    }
}

void Physics::PhysicsSystem::simulatePhysicsForPhysicComp(Physics::PhysicComponentInterface* physicComp, Core::Engine& engine)
{
    assert(physicComp != nullptr);

    // Resets colliding entities for callbacks
    std::map<CollisionComponentInterface<Physics::Shapes::Box>*, bool>::iterator it = physicComp->physicComp.collider.collidingEntities.begin();
    while (it != physicComp->physicComp.collider.collidingEntities.end())
    {
        it->second = false;
        ++it;
    }

    simulatePhysicsForASphere(physicComp, engine);

    // Sets colliding entities
    std::map<CollisionComponentInterface<Physics::Shapes::Box>*, bool>::iterator itColliding = physicComp->physicComp.collider.collidingEntities.begin();
    while (itColliding != physicComp->physicComp.collider.collidingEntities.end())
    {
        if (!itColliding->second)
        {
            physicComp->physicCompOnCollisionExit(itColliding->first);  
            itColliding = physicComp->physicComp.collider.collidingEntities.erase(itColliding);
        }
        else 
        {
            ++itColliding;
        }
    }

    simulateGravity(physicComp->physicComp, engine);

    physicComp->physicComp.collider.transform->UpdateLocalTransformMatrix();
}

void Physics::PhysicsSystem::simulatePhysicsForASphere(Physics::PhysicComponentInterface* physicComp, Core::Engine& engine)
{
    assert(physicComp != nullptr);
    
    // Update Sphere properties
    physicComp->physicComp.collider.worldCollider.center = physicComp->physicComp.collider.transform->transform.location;

    Core::Maths::Vec3 usedVelo = physicComp->physicComp.velocity * engine.deltaTime;
    physicComp->physicComp.collider.transform->transform.location = simulateCollisionsForASphere(   
                                                                                    physicComp,
                                                                                    engine, usedVelo);
}

bool Physics::PhysicsSystem::sphereCollisionWithBoxes(const Physics::Shapes::Sphere& sphere, 
                                                      const Core::Maths::Vec3& velocity,
                                                      Physics::Shapes::SegmentHit& hit,
                                                      Physics::CollisionComponentInterface<Physics::Shapes::Box>*& collidedMeshInterface)
{
    bool hasCollided = false;
    Physics::Shapes::SegmentHit segmentHit;

    Physics::Shapes::AABB aabbFromSphere;
    aabbFromSphere.setFrom(sphere);  

    Physics::Shapes::Sphere s2 = sphere;
    s2.center += velocity;
    Physics::Shapes::AABB aabbFromSphere2;
    aabbFromSphere2.setFrom(s2);  

    Physics::Shapes::AABB totalAabb = aabbFromSphere + aabbFromSphere2;
    //////////////////////////////////  

    Physics::Shapes::Segment3D seg{sphere.center, sphere.center + velocity};

    for (Physics::CollisionComponentInterface<Physics::Shapes::Box>* boxCollider : boxes)
    {
        if (boxCollider == nullptr || !boxCollider->collider.isEnabled || boxCollider->collider.isOverlap)
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

void Physics::PhysicsSystem::sphereFindOverlappingBoxes(const Physics::Shapes::Sphere& sphere, 
                                                      const Core::Maths::Vec3& lastLoc,
                                                      Physics::PhysicComponentInterface* physicComp)
{
    Physics::Shapes::Segment3D seg{sphere.center, lastLoc};
    if (seg.squaredLength() < stopMovementEpsilon)
        return;

    Physics::Shapes::SegmentHit hit;
    
    for (Physics::CollisionComponentInterface<Physics::Shapes::Box>* boxCollider : boxes)
    {
        if (boxCollider == nullptr || !boxCollider->collider.isEnabled || !boxCollider->collider.isOverlap)
            continue;

        if (Collisions::boxMovingShereCollision(boxCollider->collider.worldCollider, sphere, seg, hit))
        {
            CollisionsCallbacksSentData data {hit, physicComp, boxCollider};
            boxCollider->colliderOnOverlapEnter(data);
            physicComp->physicCompOnOverlapEnter(data);
        }
    }
}

Core::Maths::Vec3 Physics::PhysicsSystem::simulateCollisionsForASphere( 
                               Physics::PhysicComponentInterface* physicComp,
                               const Core::Engine& engine,
                               Core::Maths::Vec3& usedVelocity)
{
    Physics::Shapes::Sphere& sphere = physicComp->physicComp.collider.worldCollider;
    std::map<Physics::CollisionComponentInterface<Physics::Shapes::Box>*, bool>& collidingEntities = physicComp->physicComp.collider.collidingEntities;

    if (usedVelocity.vectorSquareLength() < stopMovementEpsilon)
    {
        return sphere.center;
    }

    Physics::Shapes::SegmentHit hit;
    hit.t = 2.f;
    Physics::CollisionComponentInterface<Physics::Shapes::Box>* collidedMeshInterface; // box

    if (sphereCollisionWithBoxes(sphere, usedVelocity, hit, collidedMeshInterface))
    {
        std::pair<std::map<CollisionComponentInterface<Physics::Shapes::Box>*, bool>::iterator, bool> it = collidingEntities.emplace(collidedMeshInterface, true);
        if (it.second)
        {
            CollisionsCallbacksSentData data {hit, physicComp, collidedMeshInterface};
            physicComp->physicCompOnCollisionEnter(data);
            collidedMeshInterface->colliderOnCollisionEnter(data);
        }

        // Physics Response
        Core::Maths::Vec3 finalLoc;
        {
            // Compute constants
            const Core::Maths::Vec3 velocityAfterContact = usedVelocity * (1.f - hit.t);
            const float dot = Core::Maths::Vec3::dotProduct(velocityAfterContact, hit.normal);

            // Compute location of the object when hitting with the object
            finalLoc = sphere.center + hit.t * usedVelocity + hit.normal * wallKnockbackEpsilon;

            // Compute next velocity of currently used velocity
            const Core::Maths::Vec3 finalLocOfUsedVelocity = sphere.center + usedVelocity - dot * hit.normal;
            const Core::Maths::Vec3 nextVelo = finalLocOfUsedVelocity - (sphere.center + hit.t * usedVelocity);

            // Compute next velocity of total velocity
            const Core::Maths::Vec3 finalLocOfTotalVelocity = sphere.center + physicComp->physicComp.velocity - dot * hit.normal;
            physicComp->physicComp.velocity = finalLocOfTotalVelocity - finalLoc;
            physicComp->physicComp.velocity *= linearDamping;

            usedVelocity = nextVelo * linearDamping;
        }

        sphereFindOverlappingBoxes(sphere, finalLoc, physicComp);

        sphere.center = finalLoc;

        return simulateCollisionsForASphere(physicComp, engine, usedVelocity);
    }
    else 
    {
        Core::Maths::Vec3 finalLoc = sphere.center + usedVelocity;
        sphereFindOverlappingBoxes(sphere, finalLoc, physicComp);
        return finalLoc;
    }
}





