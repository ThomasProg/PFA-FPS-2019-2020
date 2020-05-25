#ifndef _PHYSIC_COMPONENT_HPP_
#define _PHYSIC_COMPONENT_HPP_

#include "vec3.hpp"
#include "sphere.hpp"
#include "collisionComponent.hpp"

namespace Physics
{
    struct PhysicComponent
    {
        Core::Maths::Vec3 velocity = Core::Maths::Vec3{0.f, 0, 0};
        Physics::CollisionComponent<Sphere> collider;
        float mass = 1.f;

        // Used to test collisions with other colliders.
        // If (collisionLayer & other.layer != 0) is true, then the collision is tested.
        // By default : collides with the first and second layer.
        unsigned int collisionLayerMap = (1 << 0) | (1 << 1); 

        bool isEnabled = true;

        inline PhysicComponent(GTransform* transform);

        template<unsigned int AXIS>
        inline void setForceOnAxis(float force, const Core::Engine& engine);

        inline void setForce(const Core::Maths::Vec3& force, const Core::Engine& engine);
        inline void addForce(const Core::Maths::Vec3& force, const Core::Engine& engine);
        inline void addImpulse(const Core::Maths::Vec3& impulse);
    };
}

#include "physicComponent.inl"

#endif