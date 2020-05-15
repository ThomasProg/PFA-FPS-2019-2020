#ifndef _PHYSICS_SYSTEM_HPP_
#define _PHYSICS_SYSTEM_HPP_

#include "vec3.hpp"
#include "collisionComponent.hpp"

#include "entityID.hpp"
#include <unordered_map>

#include "sphere.hpp"
#include "box.hpp"

#include "segmentHit.hpp"
#include "segment3D.hpp"
#include "collisions.hpp"

#include <iomanip>
#include <limits>
#include <unordered_set>
#include <functional>

namespace Core
{
    class Engine;
}

namespace Renderer
{
    class Camera;
}

namespace Entity
{
    class BasicEntity;
}

namespace Physics
{
    struct PhysicComponent
    {
        Core::Maths::Vec3 velocity = Core::Maths::Vec3{0.f, 0, 0};
        Physics::CollisionComponent<Sphere> collider;

        bool isEnabled = true;
    };

    using ColliderIt = std::unordered_map<Entity::EntityID, Physics::CollisionComponent<Box>>::iterator;
    
    class PhysicsSystem
    {
    private:
        std::unordered_map<Entity::EntityID, Physics::CollisionComponent<Box>> boxes;

        static constexpr float gravityAcc = 9.81f * 0.01; 
        static constexpr float linearDamping  = 0.98f;
        static constexpr float minimalDistToGround = 0.0001f;


        // bool bHasCollided = false;
    public:
        struct PhysicsAdditionalData
        {
            std::unordered_set<Entity::EntityID> ignoredEntities;
        };

        struct CollisionsCallbacksSentData
        {
            const SegmentHit& hit;
            const Entity::EntityID& movingEntityID; // PhysicComponent
            const Entity::EntityID& encounteredEntityID; // CollisionComponent
        };

    public:
        PhysicsSystem() = default;

        decltype(boxes)::iterator addComponentTo(Entity::EntityID& entity);

        void remove(decltype(boxes)::iterator& it);

        void simulateGravity(Physics::PhysicComponent& physicComp, const Core::Engine& engine);

        Core::Maths::Vec3 collisionPhysicalResponse(Physics::PhysicComponent& physicComp, 
                                                    const Core::Maths::Vec3& startLoc, 
                                                    SegmentHit& hit);

        // returns new location
        // COLLISIONS_CALLBACKS must implement :
        // - void onCollisionEnter (CollisionsCallbacksSentData&);
        // - void onCollisionExit  (const EntityID& entityID); // TODO ?
        // - void onOverlap        (CollisionsCallbacksSentData&);
        // Warning : You shall NOT invalidate boxes iterators during the callbacks.
        // If you want to add items during the callback, use boxes.reserve().
        template<typename COLLISIONS_CALLBACKS>
        Core::Maths::Vec3 simulatePhysics(Physics::PhysicComponent& physicComp, 
                                          const Core::Maths::Vec3& startLoc, 
                                          const PhysicsAdditionalData& data,
                                          COLLISIONS_CALLBACKS& callbacks,
                                          const Entity::EntityID& physicCompEntityID);

        bool staticBoxesFirstCollision(Physics::PhysicComponent& physicComp, const Core::Maths::Vec3& startLoc, 
                                       SegmentHit& hit, const PhysicsAdditionalData& data, Entity::EntityID& collidedEntityID);

        template<typename COLLISIONS_CALLBACKS>
        void staticBoxesOverlapCollision(Physics::PhysicComponent& physicComp, 
                                         const Core::Maths::Vec3& startLoc, 
                                         const Core::Maths::Vec3& endLoc, 
                                         const PhysicsAdditionalData& data,
                                         const Entity::EntityID& physicCompEntityID,
                                         COLLISIONS_CALLBACKS& callbacks);

        bool isSegmentColliding(Renderer::Camera& camera, const Core::Maths::Vec3& forward);


        bool raycast(const Segment3D& seg, SegmentHit& hit, Entity::Entity& touchedEntity) const;


        void reset();
    };
}

#include "physicsSystem.inl"

#endif