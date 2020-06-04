#ifndef _GROUND_HPP_
#define _GROUND_HPP_

#include "collisionComponentInterface.hpp"
#include "renderableInterface.hpp"
#include "transformInterface.hpp"

#include  "demoResourceManager.hpp"

namespace Entity
{
    // Represents the ground, or walls.
    // It is static.
    class Ground final : public Physics::TransformInterface,
                         public Renderer::RenderableInterface,
                         public Physics::CollisionComponentInterface<Physics::Shapes::Box>
    {
    public:
        Ground();
        Ground(const Core::Maths::Vec4& color);

        void setResources(const DemoResourceManager&);
    };
}

#endif