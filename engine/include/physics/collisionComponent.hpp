#ifndef _COLLISION_COMPONENT_HPP_
#define _COLLISION_COMPONENT_HPP_

#include "segmentHit.hpp"

namespace Physics
{
    struct GTransform;

    template<typename COLLIDER>
    struct CollisionComponent
    {
        COLLIDER worldCollider;  
        GTransform* transform = nullptr;

        bool isEnabled = true;
        bool isOverlap = false;

    // Flags
        bool isColliding = false;
    };
}


#endif