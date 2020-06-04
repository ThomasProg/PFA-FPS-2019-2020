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
    private:
        static constexpr Core::Maths::Vec4 defaultColor = {0.301f, 0.113f, 0.f, 1};

    public:
        Ground();
        Ground(const Core::Maths::Vec4& color);

        void setResources(const DemoResourceManager&);
    };
}

#endif