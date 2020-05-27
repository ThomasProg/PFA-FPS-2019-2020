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
    public: // forward declarations
        template<typename T>
        class iterator;

        template <typename T>
        using ColliderIt = iterator<Physics::CollisionComponent<T>>;
        using PhysicCompIt = iterator<Physics::PhysicComponent>;

    private: // private variables
        std::vector<Physics::CollisionComponentInterface<Box>*> boxes;
        std::vector<ColliderIt<Box>> freeBoxesIndices;

        std::vector<Physics::PhysicComponentInterface*> physicComponents;
        std::vector<PhysicCompIt> freePhysicCompsIndices;

        static constexpr float gravityAcc = 9.81f; 
        static constexpr float linearDamping  = 0.98f;

        static constexpr float epsilon = 0.0001f;

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

    public:
        PhysicsSystem() = default;

        template<typename T = Box>
        ColliderIt<T> addCollider(Physics::CollisionComponentInterface<T>* collider);
        PhysicCompIt addPhysicComponent(Physics::PhysicComponentInterface* physicComp);

        // Removes the reference to the mesh corresponding to the iterator.
        // 
        // Inputs : The corresponding iterator.
        //
        // WARNING : 
        // - The iterator should be valid.
        //   A valid iterator is an iterator returned by addCollider() or addPhysicComponent(), 
        //   which has not been erased beforehand.
        //
        // Speed : 
        // Average Case : O(1)
        // Worst Case : O(n), n being freeMeshIndices.size()
        void erase(PhysicCompIt& it);
        void erase(ColliderIt<Box>& it);

        void simulatePhysics(Core::Engine& engine);
        void simulateGravity(PhysicComponent& physicComp, const Core::Engine& engine);

        // Core::Maths::Vec3 collisionPhysicalResponse(Physics::PhysicComponent& physicCompIt, 
        //                                             const Core::Maths::Vec3& startLoc, 
        //                                             SegmentHit& hit);

        void simulatePhysicsForPhysicComp(Physics::PhysicComponentInterface* physicComp, Core::Engine& engine);
        void simulatePhysicsForASphere(Physics::PhysicComponentInterface* physicComp, Core::Engine& engine);

        Core::Maths::Vec3 simulateCollisionsForASphere(
                                              const Physics::PhysicsSystem::PhysicsAdditionalData& data, 
                                              Physics::PhysicComponentInterface* physicCompID,
                                              const Core::Engine& engine,
                                              Core::Maths::Vec3& usedVelocity);

        void sphereFindOverlappingBoxes(const Sphere& sphere, 
                                        const Core::Maths::Vec3& velocity,
                                        const Physics::PhysicsSystem::PhysicsAdditionalData& data,
                                        Physics::PhysicComponentInterface* physicCompID);

        bool sphereCollisionWithBoxes(const Sphere& sphere, 
                                            const Core::Maths::Vec3& velocity,
                                            const Physics::PhysicsSystem::PhysicsAdditionalData& data, 
                                            SegmentHit& hit,
                                            Physics::CollisionComponentInterface<Box>*& collidedMeshInterface);

        bool raycast(const Segment3D& seg, SegmentHit& hit, Physics::CollisionComponentInterface<Box>*& touchedEntity) const;

        void reset();
    };
}

#endif