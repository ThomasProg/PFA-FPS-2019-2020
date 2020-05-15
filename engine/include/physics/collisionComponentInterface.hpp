#ifndef _COLLISION_COMPONENT_INTERFACE_HPP_ 
#define _COLLISION_COMPONENT_INTERFACE_HPP_

#include "collisionComponent.hpp"

namespace Physics
{
    class CollisionComponentInterface
    {
    public:
        Physics::ColliderIt colliderIt;
    
    public:
    
        virtual void onCollisionEnter        (const SegmentHit&) = 0;
        virtual void onCollisionExit         (const SegmentHit&) = 0;
        virtual void onOverlapEnterSelfHit   (const SegmentHit&) = 0;
        virtual void onOverlapEnterAnotherHit(const SegmentHit&) = 0;
    };
}

#endif