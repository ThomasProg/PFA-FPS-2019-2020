#ifndef _PHYSIC_COMPONENT_INTERFACE_HPP_ 
#define _PHYSIC_COMPONENT_INTERFACE_HPP_

#include "physicComponent.hpp"
#include "physicsSystem.hpp"

namespace Physics
{
    class PhysicComponentInterface
    {
    public:
        Physics::PhysicsSystem::PhysicCompIt physicCompIt;
        Physics::PhysicComponent physicComp;
    
    public:
        PhysicComponentInterface() = delete;
        PhysicComponentInterface(Physics::GTransform* transform)
            : physicComp(transform)
        {

        }
    
        virtual void physicCompOnCollisionEnter (const SegmentHit&) 
        {

        }
        virtual void physicCompOnCollisionExit  () 
        {
            
        }
        virtual void physicCompOnOverlapEnter   (const Physics::PhysicsSystem::CollisionsCallbacksSentData& data) 
        {

        }
        // virtual void physicCompOnOverlapEnterSelfHit   (const SegmentHit&) = 0;
        // virtual void physicCompOnOverlapEnterAnotherHit(const SegmentHit&) = 0;

        void addPhysics(Physics::PhysicsSystem& physicsSystem)
        {
            physicCompIt = physicsSystem.addPhysicComponent(this);
        }
    };
}
#endif