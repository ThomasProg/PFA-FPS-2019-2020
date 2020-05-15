#ifndef _COLLISION_COMPONENT_HPP_
#define _COLLISION_COMPONENT_HPP_

#include <functional>
#include "segmentHit.hpp"

namespace Physics
{
    template<typename COLLIDER>
    struct CollisionComponent
    {
        // COLLIDER localCollider;
        COLLIDER worldCollider;   

        bool isEnabled   : 1;
        bool isColliding : 1;
        bool isOverlap   : 1;

        CollisionComponent()
            : isEnabled(true), isColliding(true), isOverlap(false)
        {

        }

        // std::function<void(SegmentHit&)> onCollisionEnter = [](SegmentHit&){};
        // std::function<void(void)> onCollisionExit  = [](){};

        // // Called when this object enters the overlap of another object.
        // std::function<void(SegmentHit&)> onOverlapEnterSelfHit = [](SegmentHit&){};

        // // Called when another object enters the overlap of this object.
        // std::function<void(SegmentHit&)> onOverlapEnterAnotherHit = [](SegmentHit&){};
    };
}


#endif