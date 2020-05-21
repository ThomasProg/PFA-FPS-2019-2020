#ifndef _COLLISION_COMPONENT_HPP_
#define _COLLISION_COMPONENT_HPP_

#include "segmentHit.hpp"
#include <map>

struct Box;

namespace Physics
{
    struct GTransform;

    template<typename SHAPE>
    class CollisionComponentInterface;

    template<typename COLLIDER>
    struct CollisionComponent
    {
        COLLIDER worldCollider;  
        GTransform* transform = nullptr;

        // Defines this object's layer.
        // Default layer is 1
        unsigned int layer = 1 << 0;

        bool isEnabled = true;
        bool isOverlap = false;

        // First value of pair is an Entity, 
        // Second value is wether the entity is colliding with this one or not.
        std::map<Physics::CollisionComponentInterface<Box>*, bool> collidingEntities;

    // Flags
        bool isColliding = false;
    };
}


#endif