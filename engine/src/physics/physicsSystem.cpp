#include "physicsSystem.hpp"

#include "basicEntity.hpp"
#include "collisions.hpp"
#include "engine.hpp"
#include "camera.hpp"

// template<typename T = Box>
template<>
Physics::PhysicsSystem::ColliderIt<Box> Physics::PhysicsSystem::addCollider<Box>(Entity::EntityID& entity)
{
    boxes.emplace(entity, Physics::CollisionComponent<Box>());
    return Physics::PhysicsSystem::ColliderIt<Box>{entity, &boxes};
}


void Physics::PhysicsSystem::simulateGravity(Physics::PhysicComponent& physicComp, const Core::Engine& engine)
{
    physicComp.velocity.y -= gravityAcc * engine.deltaTime;
}

// Physics::PhysicsSystem::ColliderIt<Box> Physics::PhysicsSystem::addComponentTo(Entity::EntityID& entity)
// {
//     boxes.emplace(entity, Physics::CollisionComponent<Box>());
//     return Physics::PhysicsSystem::ColliderIt<Box>{entity, &boxes};
// }

Core::Maths::Vec3 Physics::PhysicsSystem::collisionPhysicalResponse(Physics::PhysicComponent& physicComp, 
                                                           const Core::Maths::Vec3& startLoc, 
                                                           SegmentHit& hit)
{
    // Correct Gliding : without using y axis, taking normal in account first

    // if (hit.t > 1) return Core::Maths::Vec3{0.f,0,0}; 

    Core::Maths::Vec3 newLoc = physicComp.velocity * hit.t + startLoc;

    Core::Maths::Vec3 velocityAfterContact = physicComp.velocity * (1.f - hit.t);

    // hit.normal = hit.normal.unitVector();
    float dot = Core::Maths::Vec3::dotProduct(hit.normal, velocityAfterContact);

    Core::Maths::Vec3 finalLoc = startLoc + physicComp.velocity - dot * hit.normal;
    physicComp.velocity = (physicComp.velocity - dot * hit.normal);

    // assert(hit.normal.y >= 0);
    // assert(hit.normal.x <= 0);
    // std::cout << "newLoc : " << newLoc << std::endl;
    // std::cout << "finalLoc : " << startLoc + physicComp.velocity - dot * hit.normal << std::endl;

    // physicComp.velocity += (finalLoc - newLoc) * Core::Maths::Vec3::dotProduct(physicComp.velocity, finalLoc - newLoc);

    // finalLoc.y += 0.001f;

    // finalLoc += 10 * (finalLoc - newLoc);

    // physicComp.velocity.y = 0;

    // // gliding
    // {
    //     // The plane scalar equation is : 
    //     //  n.x * x + n.y * y + n.z * z - d = 0
    //     // We want to compute the same for which x and z is the same as newLoc+b's,
    //     // but we want the object on top of the plane, so we search for y.
    //     // We know :
    //     // n = hit.normal
    //     // x = newLoc.x + b.x     z = newLoc.z + b.z
    //     // d is the signed distance of the plane to the origin, 
    //     // newLoc is part of the plane, so :
    //     // d = Core::Maths::Vec3::dotProduct(newLoc, hit.normal)
    //     // With the plane equation, we find :
    //     // y =  (- n.x * x - n.z * z - d) / n.y     IF n.y != 0

    //     // Note : We might also use b without newLoc, and the plane would be kind of relative to b + newLoc.
    //     if (hit.normal.y != 0.f)
    //     {
    //         Core::Maths::Vec3 b = physicComp.velocity * (1 - hit.t);
    //         // Core::Maths::Vec3 bUnit = b.unitVector();

    //         float d = Core::Maths::Vec3::dotProduct(newLoc, hit.normal);
    //         float y = - d - hit.normal.z * (newLoc.z + b.z) - hit.normal.x * (newLoc.x + b.x);

    //         y /= hit.normal.y;

    //         // Same X / Z the object would be without collisions, but Y is on top of the plane
    //         Core::Maths::Vec3 equiPointOnPlane = Core::Maths::Vec3{newLoc.x + b.x, y, newLoc.z + b.z};

    //         // Now that we know a direction ((equiPointOnPlane - newLoc).unitVector()),
    //         // we can set the same distance as it would be normally (b.vectorLength()).
    //         newLoc = (equiPointOnPlane - newLoc).unitVector() * b.vectorLength() + newLoc;
    //     }
    //     else 
    //     {
    //         physicComp.velocity = Core::Maths::Vec3{0.f, 0, 0};
    //     }
    // }

    // physicComp.velocity.y = 0;

    // We add a small value, because otherwises, they player would not be able to move due to constant collisions
    return finalLoc + hit.normal * minimalDistToGround;
}

// Core::Maths::Vec3 Physics::PhysicsSystem::simulatePhysics(Physics::PhysicComponent& physicComp, 
//                                                           const Core::Maths::Vec3& startLoc, 
//                                                           const PhysicsAdditionalData& data)
// {
//     if (!physicComp.isEnabled)
//     {
//         return startLoc;
//     }

//     SegmentHit hit;
//     hit.t = 1.f;

//     if (staticBoxesFirstCollision(physicComp, startLoc, hit, data))
//     {
//         Core::Maths::Vec3 newLoc = collisionPhysicalResponse(physicComp, startLoc, hit);

//         staticBoxesOverlapCollision(physicComp, startLoc, newLoc, data);
        
//         // If we don't use the "nextafter()" function at least one time, 
//         // the collision is detected, but after reajusting the sphere,
//         // the floating points errors make the sphere go through the cube.
//         // If we call the function only one time, when the player is gonna jump,
//         // the collision is detected, and will block the player, so he won't jump.
//         // newLoc.x = std::nextafter(std::nextafter(std::nextafter(newLoc.x, startLoc.x), startLoc.x), startLoc.x);
//         // newLoc.y = std::nextafter(std::nextafter(std::nextafter(newLoc.y, startLoc.y), startLoc.y), startLoc.y);
//         // newLoc.z = std::nextafter(std::nextafter(std::nextafter(newLoc.z, startLoc.z), startLoc.z), startLoc.z);

//         newLoc.x = std::nextafter(newLoc.x, startLoc.x);
//         newLoc.y = std::nextafter(newLoc.y, startLoc.y);
//         newLoc.z = std::nextafter(newLoc.z, startLoc.z);

//         physicComp.velocity *= linearDamping;

//         if (!physicComp.collider.isColliding)
//         {
//             physicComp.collider.isColliding = true;
//             if (physicComp.collider.onCollisionEnter)
//                 physicComp.collider.onCollisionEnter(hit);
//         }

//         return newLoc;
//     }
    
//     // is not colliding
//     if (physicComp.collider.isColliding)
//     {
//         physicComp.collider.isColliding = false;
//         if (physicComp.collider.onCollisionExit)
//             physicComp.collider.onCollisionExit();
//     }

//     staticBoxesOverlapCollision(physicComp, startLoc, physicComp.velocity + startLoc, data);

//     return physicComp.velocity + startLoc;
// }

bool Physics::PhysicsSystem::staticBoxesFirstCollision(Physics::PhysicComponent& physicComp, const Core::Maths::Vec3& startLoc, 
                                                       SegmentHit& hit, const PhysicsAdditionalData& data, Entity::EntityID& collidedEntityID)
{
    bool hasCollided = false;
    SegmentHit segmentHit;
    
    Segment3D seg{startLoc, startLoc + physicComp.velocity};

    for (std::pair<const Entity::EntityID, Physics::CollisionComponent<Box>>& boxCollider : boxes)
    {
        if (!boxCollider.second.isEnabled || boxCollider.second.isOverlap || data.ignoredEntities.count(boxCollider.first) > 0)
            continue;

        // boxCollider.second.worldCollider.transform = boxCollider.second.transform->transformMatrixNode->worldData; 
        //                                             // * physicComp.collider.transform->transformMatrixNode->worldData.getInverse();

        boxCollider.second.worldCollider.updateMatrixSizeFromMatrix();

        if (Collisions::boxMovingShereCollision(boxCollider.second.worldCollider, physicComp.collider.worldCollider, seg, segmentHit))
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

// void Physics::PhysicsSystem::staticBoxesOverlapCollision(Physics::PhysicComponent& physicComp, 
//                                                          const Core::Maths::Vec3& startLoc, 
//                                                          const Core::Maths::Vec3& endLoc, 
//                                                          const PhysicsAdditionalData& data)
// {   
//     Segment3D seg{startLoc, endLoc};
//     SegmentHit hit;

//     for (std::pair<const Entity::EntityID, Physics::CollisionComponent<Box>>& boxCollider : boxes)
//     {
//         if (!boxCollider.second.isEnabled || !boxCollider.second.isOverlap || data.ignoredEntities.count(boxCollider.first) > 0)
//             continue;

//         boxCollider.second.worldCollider.updateMatrixSizeFromMatrix();

//         if (Collisions::boxMovingShereCollision(boxCollider.second.worldCollider, physicComp.collider.worldCollider, seg, hit))
//         {
//             if (physicComp.collider.onOverlapEnterSelfHit)
//                 physicComp.collider.onOverlapEnterSelfHit(hit);
//             if (boxCollider.second.onOverlapEnterAnotherHit)
//                 boxCollider.second.onOverlapEnterAnotherHit(hit);
//         }
//     }
// }

bool Physics::PhysicsSystem::isSegmentColliding(Renderer::Camera& camera, const Core::Maths::Vec3& forward)
{
    for (std::pair<const Entity::EntityID, Physics::CollisionComponent<Box>>& boxCollider : boxes)
    {
        SegmentHit hit;
        Segment3D seg;
        seg.p1 = {0.f, 0, 0};
        seg.p2 = {0.f, 0, -10};
        seg.p1 = camera.transform.transformMatrixNode->worldData * seg.p1;
        seg.p2 = camera.transform.transformMatrixNode->worldData * seg.p2;

        if (Collisions::boxSegmentCollision(boxCollider.second.worldCollider, seg, hit))
        {
            return true;
        }
    }
    return false;
}

bool Physics::PhysicsSystem::raycast(const Segment3D& seg, SegmentHit& hit, Entity::EntityID& touchedEntity) const
{
    hit.t = 2.f;

    for (const std::pair<const Entity::EntityID, const Physics::CollisionComponent<Box>>& boxCollider : boxes)
    {
        SegmentHit tempHit;
        if (Collisions::boxSegmentCollision(boxCollider.second.worldCollider, seg, tempHit))
        {
            if (tempHit.t < hit.t)
            {
                touchedEntity = boxCollider.first;
                hit = tempHit;
            }
        }
    }

    return hit.t >= 0.f && hit.t <= 1.f;
}

void Physics::PhysicsSystem::reset()
{
    boxes.clear();
}
