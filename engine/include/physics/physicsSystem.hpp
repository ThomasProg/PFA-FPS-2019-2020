#ifndef _PHYSICS_SYSTEM_HPP_
#define _PHYSICS_SYSTEM_HPP_

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>

#include "box.hpp"


#include "segmentHit.hpp"

class Sphere;
class Segment3D;

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
    
    class PhysicsSystem
    {
    private:
        std::vector<Physics::CollisionComponentInterface<Box>*> boxes;
        std::vector<Physics::PhysicComponentInterface*> physicComponents;

        static constexpr float gravityAcc = 9.81f; 
        static constexpr float linearDamping  = 0.98f;
        static constexpr float minimalDistToGround = 0.0001f;

    public:
        // reserve vector to prevent too much move
        unsigned int nextNbOverlapHint = 0;

        struct PhysicsAdditionalData
        {
            std::unordered_set<Physics::CollisionComponentInterface<Box>*> ignoredEntities;
        };

        struct CollisionsCallbacksSentData
        {
            const SegmentHit& hit;
            const Physics::PhysicComponentInterface* movingEntityID; // PhysicComponent
            const Physics::CollisionComponentInterface<Box>* encounteredEntityID; // CollisionComponent
        };

        template<typename T>
        class iterator
        {
        private:
            unsigned int arrayIndex = 0;

        public:
            iterator() = default;

            iterator(unsigned int index)
                : arrayIndex(index)
            {

            }

            friend PhysicsSystem;
        };

        template <typename T>
        using ColliderIt = iterator<Physics::CollisionComponent<T>>;
        using PhysicCompIt = iterator<Physics::PhysicComponent>;


    public:
        PhysicsSystem() = default;

        template<typename T = Box>
        ColliderIt<T> addCollider(Physics::CollisionComponentInterface<T>* collider);
        PhysicCompIt addPhysicComponent(Physics::PhysicComponentInterface* physicComp);

        void simulatePhysics(Core::Engine& engine);
        void simulateGravity(PhysicComponent& physicComp, const Core::Engine& engine);

        // Core::Maths::Vec3 collisionPhysicalResponse(Physics::PhysicComponent& physicCompIt, 
        //                                             const Core::Maths::Vec3& startLoc, 
        //                                             SegmentHit& hit);

        // void simulatePhysics(Physics::PhysicComponentInterface* physicComp, 
        //                      const PhysicsAdditionalData& data,
        //                      const Core::Maths::Vec3& leftVelocity);

        void simulatePhysicsForPhysicComp(Physics::PhysicComponentInterface* physicComp, Core::Engine& engine);
        void simulatePhysicsForASphere(Physics::PhysicComponentInterface* physicComp, Core::Engine& engine);

        Core::Maths::Vec3 simulateCollisionsForASphere(
                                              const Physics::PhysicsSystem::PhysicsAdditionalData& data, 
                                              Physics::PhysicComponentInterface* physicCompID);

        void sphereFindOverlappingBoxes(const Sphere& sphere, 
                                        const Core::Maths::Vec3& velocity,
                                        const Physics::PhysicsSystem::PhysicsAdditionalData& data,
                                        Physics::PhysicComponentInterface* physicCompID);

        bool sphereCollisionWithBoxes(const Sphere& sphere, 
                                            const Core::Maths::Vec3& velocity,
                                            const Physics::PhysicsSystem::PhysicsAdditionalData& data, 
                                            SegmentHit& hit,
                                            Physics::CollisionComponentInterface<Box>*& collidedMeshInterface);


        // bool staticBoxesFirstCollision(Physics::PhysicComponent& physicComp, const Core::Maths::Vec3& startLoc, 
        //                                SegmentHit& hit, const PhysicsAdditionalData& data, 
        //                                Physics::CollisionComponentInterface<Box>*& collidedEntityID,
        //                                const Core::Maths::Vec3& velocity);

        // void staticBoxesOverlapCollision(Physics::PhysicComponentInterface* physicComp, 
        //                                  const Core::Maths::Vec3& startLoc, 
        //                                  const Core::Maths::Vec3& endLoc, 
        //                                  const PhysicsAdditionalData& data);

        // bool isSegmentColliding(Renderer::Camera& camera, const Core::Maths::Vec3& forward);


        bool raycast(const Segment3D& seg, SegmentHit& hit, Physics::CollisionComponentInterface<Box>*& touchedEntity) const;

        void reset();
    };
}

#endif