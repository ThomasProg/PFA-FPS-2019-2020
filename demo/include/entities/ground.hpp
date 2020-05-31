#ifndef _GROUND_HPP_
#define _GROUND_HPP_

#include "staticObject.hpp"

#include  "demoResourceManager.hpp"

namespace Entity
{
    class Ground final : public Physics::TransformInterface,
                         public Renderer::RenderableInterface,
                         public Physics::CollisionComponentInterface<Physics::Shapes::Box>
    {
    public:
        Ground() 
            : Renderer::RenderableInterface(&transform),
              Physics::CollisionComponentInterface<Physics::Shapes::Box>(&transform)
        {
            mesh.color = Core::Maths::Vec4{0.835f, 0.650f, 0.384f,1};
        }

        Ground(const Core::Maths::Vec4& color) 
            : Renderer::RenderableInterface(&transform),
              Physics::CollisionComponentInterface<Physics::Shapes::Box>(&transform)
        {
            mesh.color = color;
        }

        void setResources(const DemoResourceManager&);
    };
}

#endif