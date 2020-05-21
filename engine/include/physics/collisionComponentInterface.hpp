#ifndef _COLLISION_COMPONENT_INTERFACE_HPP_ 
#define _COLLISION_COMPONENT_INTERFACE_HPP_

#include "collisionComponent.hpp"
#include "physicsSystem.hpp"

namespace Physics
{
    template<typename SHAPE>
    class CollisionComponentInterface
    {
    public:
        Physics::PhysicsSystem::ColliderIt<SHAPE> colliderIt;
        Physics::CollisionComponent<SHAPE> collider;
    
    public:
    
        virtual void colliderOnCollisionEnter (const SegmentHit&) = 0;
        virtual void colliderOnCollisionExit  () = 0;
        virtual void colliderOnOverlapEnter   (const Physics::PhysicsSystem::CollisionsCallbacksSentData& data) 
        {

        }

        void addCollisions(Physics::PhysicsSystem& physicsSystem)
        {
            colliderIt = physicsSystem.addCollider<Box>(this);
        }
    };
}

#endif