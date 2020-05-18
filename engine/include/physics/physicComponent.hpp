#ifndef _PHYSIC_COMPONENT_HPP_
#define  _PHYSIC_COMPONENT_HPP_

#include "vec3.hpp"
#include "sphere.hpp"
#include "collisionComponent.hpp"

namespace Physics
{
    struct PhysicComponent
    {
        Core::Maths::Vec3 velocity = Core::Maths::Vec3{0.f, 0, 0};
        Physics::CollisionComponent<Sphere> collider;

        bool isEnabled = true;
    };
}

#endif