#include "physicComponent.hpp"

#include "engine.hpp"

inline Physics::PhysicComponent::PhysicComponent(GTransform* transform)
    : collider(transform)
{

}

template<unsigned int AXIS>
inline void Physics::PhysicComponent::setForceOnAxis(float force, const Core::Engine& engine)
{
    // m / s         (N / kg) * s   = kg * m * s^(-2) * s = m / s
    velocity[AXIS] = (force / mass) * engine.deltaTime;
}

inline void Physics::PhysicComponent::setForce(const Core::Maths::Vec3& force, const Core::Engine& engine)
{
    // m / s         (N / kg) * s   = kg * m * s^(-2) * s = m / s
    velocity = (force / mass) * engine.deltaTime;
}

inline void Physics::PhysicComponent::addForce(const Core::Maths::Vec3& force, const Core::Engine& engine)
{
    // m / s         (N / kg) * s   = kg * m * s^(-2) * s = m / s
    velocity += (force / mass) * engine.deltaTime;
}

inline void Physics::PhysicComponent::addImpulse(const Core::Maths::Vec3& impulse)
{
    // An impulse is not a force; it will happen only one time.
    // That's why we don't need the deltaTime, which is useful only for forces during in time.
    // If we used the delta time here, the impulse wouldn't be the same each time we use it, which is not what we want.
    // Adding an impulse is just adding velocity.
    // m / s    m / s
    velocity += impulse;
}

