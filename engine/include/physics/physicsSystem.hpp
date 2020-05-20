#ifndef _PHYSICS_SYSTEM_HPP_
#define _PHYSICS_SYSTEM_HPP_

#include "entityID.hpp"

#include "collisions.hpp"
#include "segmentHit.hpp"
#include "segment3D.hpp"
#include "sphere.hpp"
#include "box.hpp"
#include "vec3.hpp"

#include <unordered_map>
#include <iomanip>
#include <limits>
#include <unordered_set>
#include <set>
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
    template<typename SHAPE>
    class CollisionComponentInterface;
    template<typename SHAPE>
    class CollisionComponent;

    class PhysicComponentInterface;
    class PhysicComponent;

    // using ColliderIt = std::unordered_map<Entity::EntityID, Physics::CollisionComponent<Box>>::iterator;
    
    class PhysicsSystem
    {
    private:
        // std::unordered_map<Entity::EntityID, Physics::CollisionComponent<Box>> boxes;
        // std::unordered_map<Entity::EntityID, Physics::PhysicComponent> physicComponents;
        std::vector<Physics::CollisionComponentInterface<Box>*> boxes;
        std::vector<Physics::PhysicComponentInterface*> physicComponents;

        static constexpr float gravityAcc = 9.81f * 0.01; 
        static constexpr float linearDamping  = 0.98f;
        static constexpr float minimalDistToGround = 0.0001f;

    public:
        // reserve vector to prevent too much move
        unsigned int nextNbOverlapHint = 0;

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

        struct CollisionsCallbacksSentDataCpy
        {
            const SegmentHit hit;
            const Entity::EntityID movingEntityID; // PhysicComponent
            const Entity::EntityID encounteredEntityID; // CollisionComponent

            inline CollisionsCallbacksSentDataCpy(const SegmentHit& hit, 
                                                  const Entity::EntityID& movingEntityID, 
                                                  const Entity::EntityID& encounteredEntityID)
                : hit(hit), movingEntityID(movingEntityID), encounteredEntityID(encounteredEntityID)
            {

            }
        };

        template<typename T>
        class iterator
        {
        private:
            unsigned int arrayIndex = 0;

            // Entity::EntityID entityID;
            // std::unordered_map<Entity::EntityID, T>* mapPtr = nullptr;

        public:
            iterator() = default;

            iterator(unsigned int index)
                : arrayIndex(index)
            {

            }

            // iterator(Entity::EntityID& entityID, std::unordered_map<Entity::EntityID, T>* container)
            //     : entityID(entityID), mapPtr(container)
            // {

            // }

            // T& operator*()
            // {
            //     assert(mapPtr != nullptr);                
            //     return mapPtr->at(entityID);
            // }

            // T* operator->()
            // {
            //     assert(mapPtr != nullptr);
            //     return &mapPtr->at(entityID);
            // }

            // const T* operator->() const
            // {
            //     assert(mapPtr != nullptr);
            //     return &mapPtr->at(entityID);
            // }

            // bool isValid() const noexcept
            // {
            //     return mapPtr != nullptr;
            // }

            // void erase()
            // {
            //     assert(mapPtr != nullptr);
            //     mapPtr->erase(entityID);
            //     mapPtr = nullptr;
            // }

            friend PhysicsSystem;
        };

        template <typename T>
        using ColliderIt = iterator<Physics::CollisionComponent<T>>;
        using PhysicCompIt = iterator<Physics::PhysicComponent>;


    public:
        PhysicsSystem() = default;

        template<typename T = Box>
        ColliderIt<T> addCollider(Entity::EntityID& entity);
        // template<typename T = Sphere>
        // inline PhysicCompIt addPhysicComponent(Entity::EntityID& entity);
        inline PhysicCompIt addPhysicComponent(Physics::PhysicComponentInterface* physicComp);

        // template <typename T>
        // void loadColliderItContainer(ColliderIt<T>& it, const Entity::EntityID& entityID);

        // inline void loadPhysicComponentItContainer(PhysicCompIt& it, const Entity::EntityID& entityID)
        // {
        //     it.entityID = entityID;
        //     it.mapPtr = &physicComponents;
        // }

        // COLLISIONS_CALLBACKS must implement :
        // - void onCollisionEnter (CollisionsCallbacksSentData&);
        // - void onCollisionExit  (const EntityID& entityID); // TODO ?
        // - void onOverlap        (CollisionsCallbacksSentData&);
        // Warning : You shall NOT invalidate boxes iterators during the callbacks.
        // If you want to add items during the callback, use boxes.reserve().
        template<typename COLLISIONS_CALLBACKS>
        void simulate(COLLISIONS_CALLBACKS& callbacks, Core::Engine& engine);

        void simulateGravity(PhysicComponent& physicComp, const Core::Engine& engine);

        Core::Maths::Vec3 collisionPhysicalResponse(Physics::PhysicComponent& physicCompIt, 
                                                    const Core::Maths::Vec3& startLoc, 
                                                    SegmentHit& hit);

        // COLLISIONS_CALLBACKS must implement :
        // - void onCollisionEnter (CollisionsCallbacksSentData&);
        // - void onCollisionExit  (const EntityID& entityID); // TODO ?
        // - void onOverlap        (CollisionsCallbacksSentData&);
        // Warning : You shall NOT invalidate boxes iterators during the callbacks.
        // If you want to add items during the callback, use boxes.reserve().
        template<typename COLLISIONS_CALLBACKS>
        void simulatePhysics(std::pair<const Entity::EntityID, Physics::PhysicComponent>& physicComp, 
                             const PhysicsAdditionalData& data,
                             const Core::Maths::Vec3& leftVelocity,
                             COLLISIONS_CALLBACKS& callbacks);


        template<typename COLLISIONS_CALLBACKS>
        inline Core::Maths::Vec3 simulatePhysicsForASphere(const Sphere& sphere, 
                                              const Physics::PhysicsSystem::PhysicsAdditionalData& data, 
                                              std::map<Entity::EntityID, bool>& collidingEntities,
                                              Core::Maths::Vec3& leftVelocity,
                                              const Entity::EntityID& physicCompID,
                                              COLLISIONS_CALLBACKS& callbacks);

        template<typename COLLISIONS_CALLBACKS>
        void sphereFindOverlappingBoxes(const Sphere& sphere, 
                                        const Core::Maths::Vec3& velocity,
                                        const Physics::PhysicsSystem::PhysicsAdditionalData& data,
                                        const Entity::EntityID& physicCompID,
                                        COLLISIONS_CALLBACKS& callbacks);

        inline bool sphereCollisionWithBoxes(const Sphere& sphere, 
                                            const Core::Maths::Vec3& velocity,
                                            const Physics::PhysicsSystem::PhysicsAdditionalData& data, 
                                            SegmentHit& hit,
                                            Physics::CollisionComponentInterface<Box>*& collidedMeshInterface);


        bool staticBoxesFirstCollision(Physics::PhysicComponent& physicComp, const Core::Maths::Vec3& startLoc, 
                                       SegmentHit& hit, const PhysicsAdditionalData& data, Entity::EntityID& collidedEntityID,
                                       const Core::Maths::Vec3& velocity);

        template<typename COLLISIONS_CALLBACKS>
        void staticBoxesOverlapCollision(Physics::PhysicComponent& physicComp, 
                                         const Core::Maths::Vec3& startLoc, 
                                         const Core::Maths::Vec3& endLoc, 
                                         const PhysicsAdditionalData& data,
                                         const Entity::EntityID& physicCompEntityID,
                                         COLLISIONS_CALLBACKS& callbacks);

        bool isSegmentColliding(Renderer::Camera& camera, const Core::Maths::Vec3& forward);


        bool raycast(const Segment3D& seg, SegmentHit& hit, Entity::EntityID& touchedEntity) const;

        void reset();
    };
}

#include "physicsSystem.inl"

#endif