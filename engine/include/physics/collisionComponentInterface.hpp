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
        CollisionComponentInterface() = delete;
        CollisionComponentInterface(Physics::GTransform* transform);

        virtual void colliderOnCollisionEnter (const Physics::PhysicsSystem::CollisionsCallbacksSentData&)
        {
            
        }
        virtual void colliderOnCollisionExit  ()
        {

        }
        virtual void colliderOnOverlapEnter   (const Physics::PhysicsSystem::CollisionsCallbacksSentData& data) 
        {

        }

        void addCollisions(Physics::PhysicsSystem& physicsSystem)
        {
            colliderIt = physicsSystem.addCollider<Physics::Shapes::Box>(this);
        }
    };
}

#include "collisionComponentInterface.inl"

#endif