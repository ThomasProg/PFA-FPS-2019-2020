#include "collisionComponentInterface.hpp"

template<typename SHAPE>
Physics::CollisionComponentInterface<SHAPE>::CollisionComponentInterface(Physics::GTransform* transform)
    : collider(transform)
{

}