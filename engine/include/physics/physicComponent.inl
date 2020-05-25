#include "physicComponent.hpp"

#include "engine.hpp"

inline Physics::PhysicComponent::PhysicComponent(GTransform* transform)
    : collider(transform)
{

}

template<unsigned int AXIS>
inline void Physics::PhysicComponent::setForceOnAxis(float force, const Core::Engine& engine)
{
    velocity[AXIS] = force / mass;
}

inline void Physics::PhysicComponent::setForce(const Core::Maths::Vec3& force, const Core::Engine& engine)
{
    velocity = force / mass;
}

inline void Physics::PhysicComponent::addForce(const Core::Maths::Vec3& force, const Core::Engine& engine)
{
    velocity += force / mass;
}

inline void Physics::PhysicComponent::addImpulse(const Core::Maths::Vec3& impulse)
{
    velocity += impulse;
}

