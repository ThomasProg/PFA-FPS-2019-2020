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

bool Physics::PhysicsSystem::raycast(const Segment3D& seg, SegmentHit& hit, Physics::CollisionComponentInterface<Physics::Shapes::Box>*& touchedEntity) const
{
    hit.t = 2.f;

    for (Physics::CollisionComponentInterface<Physics::Shapes::Box>* boxCollider : boxes)
    {
        if (boxCollider == nullptr)
            continue;

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

        box->collider.worldCollider.transform = box->collider.transform->transformMatrixNode->worldData;
        box->collider.aabb.setFrom(box->collider.worldCollider);
    }

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

    // Resets colliding entities
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
    physicComp->physicComp.collider.transform->transformMatrixNode->setDirtySelfAndChildren();
    physicComp->physicComp.collider.transform->transformMatrixNode->cleanUpdate();
}

void Physics::PhysicsSystem::simulatePhysicsForASphere(Physics::PhysicComponentInterface* physicComp, Core::Engine& engine)
{
    assert(physicComp != nullptr);
    
    // Update Sphere properties
    physicComp->physicComp.collider.worldCollider.center = physicComp->physicComp.collider.transform->transform.location;

    Physics::PhysicsSystem::PhysicsAdditionalData playerIgnoreData;
    playerIgnoreData.ignoredEntities.emplace(nullptr);
    Core::Maths::Vec3 usedVelo = physicComp->physicComp.velocity * engine.deltaTime;
    physicComp->physicComp.collider.transform->transform.location = simulateCollisionsForASphere( 
                                                                                    playerIgnoreData,  
                                                                                    physicComp,
                                                                                    engine, usedVelo);
}

bool Physics::PhysicsSystem::sphereCollisionWithBoxes(const Physics::Shapes::Sphere& sphere, 
                                                      const Core::Maths::Vec3& velocity,
                                                      const Physics::PhysicsSystem::PhysicsAdditionalData& data, 
                                                      SegmentHit& hit,
                                                      Physics::CollisionComponentInterface<Physics::Shapes::Box>*& collidedMeshInterface)
{
    bool hasCollided = false;
    SegmentHit segmentHit;

    // TODO : Remove and replace args
    Physics::Shapes::AABB aabbFromSphere;
    aabbFromSphere.setFrom(sphere);  

    Physics::Shapes::Sphere s2 = sphere;
    s2.center += velocity;
    Physics::Shapes::AABB aabbFromSphere2;
    aabbFromSphere2.setFrom(s2);  

    Physics::Shapes::AABB totalAabb = aabbFromSphere + aabbFromSphere2;
    //////////////////////////////////  

    Segment3D seg{sphere.center, sphere.center + velocity};

    for (Physics::CollisionComponentInterface<Physics::Shapes::Box>* boxCollider : boxes)
    {
        if (boxCollider == nullptr || !boxCollider->collider.isEnabled || boxCollider->collider.isOverlap)// || data.ignoredEntities.count(boxCollider.first) > 0) // TODO : ignored entities
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

    // for (std::pair<const Entity::EntityID, Physics::CollisionComponent<Physics::Shapes::Box>>& boxCollider : boxes)
    for (Physics::CollisionComponentInterface<Physics::Shapes::Box>* boxCollider : boxes)
    {
        if (boxCollider == nullptr || !boxCollider->collider.isEnabled || !boxCollider->collider.isOverlap || data.ignoredEntities.count(boxCollider) > 0)
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
                               Physics::PhysicComponentInterface* physicComp,
                               const Core::Engine& engine,
                               Core::Maths::Vec3& usedVelocity)
{
    Physics::Shapes::Sphere& sphere = physicComp->physicComp.collider.worldCollider;
    std::map<Physics::CollisionComponentInterface<Physics::Shapes::Box>*, bool>& collidingEntities = physicComp->physicComp.collider.collidingEntities;

    if (usedVelocity.vectorSquareLength() < 0.00001f)
    {
        return sphere.center;
    }

    SegmentHit hit;
    hit.t = 2.f;
    // Entity::EntityID collidedEntity; // box
    Physics::CollisionComponentInterface<Physics::Shapes::Box>* collidedMeshInterface; // box

    if (sphereCollisionWithBoxes(sphere, usedVelocity, data, hit, collidedMeshInterface))
    {
        std::pair<std::map<CollisionComponentInterface<Physics::Shapes::Box>*, bool>::iterator, bool> it = collidingEntities.emplace(collidedMeshInterface, true);
        if (!it.second)
        {
            physicComp->physicCompOnCollisionEnter(hit);
            collidedMeshInterface->colliderOnCollisionEnter(hit);
        }

        Core::Maths::Vec3 velocityAfterContact = usedVelocity * (1.f - hit.t);
        float dot = Core::Maths::Vec3::dotProduct(velocityAfterContact, hit.normal);
        Core::Maths::Vec3 finalLoc = sphere.center + hit.t * usedVelocity + hit.normal * 0.00001f;

        {
            Core::Maths::Vec3 otherFinalLoc = sphere.center + usedVelocity - dot * hit.normal;
            usedVelocity = otherFinalLoc - (sphere.center + hit.t * usedVelocity);
        }

        {
            // TODO : verify if correct 
            float dot2 = Core::Maths::Vec3::dotProduct(physicComp->physicComp.velocity/* - usedVelocity * hit.t*/, hit.normal);
            Core::Maths::Vec3 otherFinalLoc = sphere.center + physicComp->physicComp.velocity - dot2 * hit.normal;
            physicComp->physicComp.velocity = otherFinalLoc - (sphere.center + hit.t * usedVelocity);
            physicComp->physicComp.velocity *= linearDamping;
        }

        usedVelocity *= 0.9f;

        // // Core::Maths::Vec3 velocityAfterContact = usedVelocity * (1.f - hit.t);
        // // float dot = Core::Maths::Vec3::dotProduct(hit.normal, velocityAfterContact);
        // // Core::Maths::Vec3 finalLoc = sphere.center + usedVelocity - dot * hit.normal * 1.001;

        // Core::Maths::Vec3 velocityAfterContact = usedVelocity * (1.f - hit.t);
        // float dot = Core::Maths::Vec3::dotProduct(hit.normal, velocityAfterContact);
        // Core::Maths::Vec3 finalLoc = sphere.center + usedVelocity - dot * hit.normal;

        // std::cout << physicComp->physicComp.velocity  << std::endl;

        // std::cout << - dot << std::endl;
        // float dot2 = Core::Maths::Vec3::dotProduct(hit.normal, physicComp->physicComp.velocity);
        // if (dot2 > 0) 
        //     return sphere.center;
        // Core::Maths::Vec3 np = sphere.center - dot2 * hit.normal;
        // Core::Maths::Vec3 nb = sphere.center + 2.f * (np - sphere.center);
        
        // physicComp->physicComp.velocity = nb - (sphere.center + usedVelocity * hit.t);
        // std::cout << physicComp->physicComp.velocity  << std::endl;
        // // physicComp->physicComp.velocity -= dot*hit.normal;
        // physicComp->physicComp.velocity *= 0.9f;
        // return sphere.center;
        // // usedVelocity -= dot * hit.normal;
        // usedVelocity *= 0.9f;

        // return sphere.center;

        sphereFindOverlappingBoxes(sphere, finalLoc, data, physicComp);

        sphere.center = finalLoc;

        return simulateCollisionsForASphere(data, physicComp, engine, usedVelocity);
    }
    else 
    {
        Core::Maths::Vec3 finalLoc = sphere.center + usedVelocity;
        sphereFindOverlappingBoxes(sphere, finalLoc, data, physicComp);
        return finalLoc;
    }
}





