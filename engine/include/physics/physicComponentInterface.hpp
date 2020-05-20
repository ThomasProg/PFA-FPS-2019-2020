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
        Physics::PhysicsSystem::PhysicComponent physicComp;
    
    public:
    
        virtual void onCollisionEnter        (const SegmentHit&) = 0;
        virtual void onCollisionExit         () = 0;
        virtual void onOverlapEnterSelfHit   (const SegmentHit&) = 0;
        virtual void onOverlapEnterAnotherHit(const SegmentHit&) = 0;
    };
}
#endif