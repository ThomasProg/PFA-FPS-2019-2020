#include "transform.hpp"

inline Core::Maths::Vec3 Physics::Transform::getForwardVector()
{
    return Core::Maths::Vec3{(float) (sin(-rotation.y) * sin(-rotation.x)),
                             (float) (sin(-rotation.y) * cos(-rotation.x)),
                           - (float) (cos(-rotation.y))};
}

inline Core::Maths::Vec3 Physics::Transform::getForwardXZVector()
{
    return Core::Maths::Vec3{(float) sin(-rotation.y),
                                0.f,
                                - (float) cos(-rotation.y)};
}

inline Core::Maths::Vec3 Physics::Transform::getRightXZVector()
{
    return Core::Maths::Vec3{(float) sin(-rotation.y + M_PI / 2),
                                0.f,
                                - (float) cos(-rotation.y + M_PI / 2)};
}
