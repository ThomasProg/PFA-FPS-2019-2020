#include "physicsSystem.hpp"

#include <vector>

template<typename COLLISIONS_CALLBACKS>
Core::Maths::Vec3 Physics::PhysicsSystem::simulatePhysics(Physics::PhysicComponent& physicComp, 
                                                          const Core::Maths::Vec3& startLoc, 
                                                          const Physics::PhysicsSystem::PhysicsAdditionalData& data,
                                                          COLLISIONS_CALLBACKS& callbacks,
                                                          const Entity::EntityID& physicCompEntityID)
{
    if (!physicComp.isEnabled)
    {
        return startLoc;
    }

    SegmentHit hit;
    hit.t = 2.f;

    Entity::EntityID collidedEntity; // box

    if (staticBoxesFirstCollision(physicComp, startLoc, hit, data, collidedEntity))
    {
        Core::Maths::Vec3 newLoc = collisionPhysicalResponse(physicComp, startLoc, hit);

        staticBoxesOverlapCollision(physicComp, startLoc, newLoc, data, physicCompEntityID, callbacks);
        
        // If we don't use the "nextafter()" function at least one time, 
        // the collision is detected, but after reajusting the sphere,
        // the floating points errors make the sphere go through the cube.
        // If we call the function only one time, when the player is gonna jump,
        // the collision is detected, and will block the player, so he won't jump.
        // newLoc.x = std::nextafter(std::nextafter(std::nextafter(newLoc.x, startLoc.x), startLoc.x), startLoc.x);
        // newLoc.y = std::nextafter(std::nextafter(std::nextafter(newLoc.y, startLoc.y), startLoc.y), startLoc.y);
        // newLoc.z = std::nextafter(std::nextafter(std::nextafter(newLoc.z, startLoc.z), startLoc.z), startLoc.z);

        newLoc.x = std::nextafter(newLoc.x, startLoc.x);
        newLoc.y = std::nextafter(newLoc.y, startLoc.y);
        newLoc.z = std::nextafter(newLoc.z, startLoc.z);

        physicComp.velocity *= linearDamping;

        if (!physicComp.collider.isColliding)
        {
            physicComp.collider.isColliding = true;

            // Callback
            CollisionsCallbacksSentData collisionsCallbacksSentData
            {
                std::move(hit),
                physicCompEntityID,
                collidedEntity
            };
            callbacks.onCollisionEnter(collisionsCallbacksSentData);
        }

        return newLoc;
    }
    
    // is not colliding
    if (physicComp.collider.isColliding)
    {
        physicComp.collider.isColliding = false;

        // Callback
        callbacks.onCollisionExit(physicCompEntityID);
    }

    staticBoxesOverlapCollision(physicComp, startLoc, physicComp.velocity + startLoc, data, physicCompEntityID, callbacks);

    return physicComp.velocity + startLoc;
}

template<typename COLLISIONS_CALLBACKS>
void Physics::PhysicsSystem::staticBoxesOverlapCollision(Physics::PhysicComponent& physicComp, 
                                                         const Core::Maths::Vec3& startLoc, 
                                                         const Core::Maths::Vec3& endLoc, 
                                                         const PhysicsAdditionalData& data,
                                                         const Entity::EntityID& physicCompEntityID,
                                                         COLLISIONS_CALLBACKS& callbacks)
{   
    Segment3D seg{startLoc, endLoc};
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