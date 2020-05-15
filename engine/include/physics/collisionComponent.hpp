#ifndef _COLLISION_COMPONENT_HPP_
#define _COLLISION_COMPONENT_HPP_

#include <functional>
#include "segmentHit.hpp"

namespace Physics
{
    template<typename COLLIDER>
    struct CollisionComponent
    {
        COLLIDER worldCollider;   

        bool isEnabled   : 1;
        bool isColliding : 1;
        bool isOverlap   : 1;

        inline CollisionComponent()
            : isEnabled(true), isColliding(true), isOverlap(false)
        {

        }
    };
}


#endif