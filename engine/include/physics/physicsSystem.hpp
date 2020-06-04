#ifndef _PHYSICS_SYSTEM_HPP_
#define _PHYSICS_SYSTEM_HPP_

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>

#include "box.hpp"


#include "segmentHit.hpp"

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
    namespace Shapes
    {
        class Segment3D;
        class Sphere;
    }

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
        std::vector<Physics::CollisionComponentInterface<Physics::Shapes::Box>*> boxes;
        std::vector<ColliderIt<Physics::Shapes::Box>> freeBoxesIndices;

        std::vector<Physics::PhysicComponentInterface*> physicComponents;
        std::vector<PhysicCompIt> freePhysicCompsIndices;

        static constexpr float gravityAcc = 9.81f; 
        static constexpr float linearDamping  = 0.90f;

        static constexpr float wallKnockbackEpsilon = 0.001;
        static constexpr float stopMovementEpsilon = 0.0001;

    public:
        // reserve vector to prevent too much move
        unsigned int nextNbOverlapHint = 0;

        struct CollisionsCallbacksSentData
        {
            const Physics::Shapes::SegmentHit& hit;
            const Physics::PhysicComponentInterface* movingEntityID; // PhysicComponent
            const Physics::CollisionComponentInterface<Physics::Shapes::Box>* encounteredEntityID; // CollisionComponent
        };

        template<typename T>
        class iterator
        {
        private:
            unsigned int arrayIndex = 0;

        public:
            iterator() = default;
            iterator(unsigned int index);

            friend PhysicsSystem;
        };

    public:
        PhysicsSystem() = default;

        template<typename T = Physics::Shapes::Box>
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
        void erase(ColliderIt<Physics::Shapes::Box>& it);

        void simulatePhysics(Core::Engine& engine);
        void simulateGravity(PhysicComponent& physicComp, const Core::Engine& engine);

        void simulatePhysicsForPhysicComp(Physics::PhysicComponentInterface* physicComp, Core::Engine& engine);
        void simulatePhysicsForASphere(Physics::PhysicComponentInterface* physicComp, Core::Engine& engine);

        Core::Maths::Vec3 simulateCollisionsForASphere(
                                              Physics::PhysicComponentInterface* physicCompID,
                                              const Core::Engine& engine,
                                              Core::Maths::Vec3& usedVelocity);

        void sphereFindOverlappingBoxes(const Physics::Shapes::Sphere& sphere, 
                                        const Core::Maths::Vec3& velocity,
                                        Physics::PhysicComponentInterface* physicCompID);

        bool sphereCollisionWithBoxes(const Physics::Shapes::Sphere& sphere, 
                                            const Core::Maths::Vec3& velocity,
                                            Physics::Shapes::SegmentHit& hit,
                                            Physics::CollisionComponentInterface<Physics::Shapes::Box>*& collidedMeshInterface);

        bool raycast(const Physics::Shapes::Segment3D& seg, Physics::Shapes::SegmentHit& hit, 
                     Physics::CollisionComponentInterface<Physics::Shapes::Box>*& touchedEntity,
                     unsigned int consideredLayers = 0xFFFFFFFF) const;

        void reset();
    };
}

#include "physicsSystem.inl"

#endif