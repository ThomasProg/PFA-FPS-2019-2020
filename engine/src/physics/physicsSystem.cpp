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

template<> 
void Physics::PhysicsSystem::loadColliderItContainer<Box>(ColliderIt<Box>& it, const Entity::EntityID& entityID)
{
    it.entityID = entityID;
    it.mapPtr = &boxes;
}


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
                                                       SegmentHit& hit, const PhysicsAdditionalData& data, Entity::EntityID& collidedEntityID,
                                                       const Core::Maths::Vec3& velocity)
{
    bool hasCollided = false;
    SegmentHit segmentHit;
    
    Segment3D seg{startLoc, startLoc + velocity};

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
    physicComponents.clear();
}
