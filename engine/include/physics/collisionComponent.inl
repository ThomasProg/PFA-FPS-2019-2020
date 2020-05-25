#include "collisionComponent.hpp"

template<typename COLLIDER>
inline Physics::CollisionComponent<COLLIDER>::CollisionComponent(Physics::GTransform* transform)
    : transform(transform)
{

}