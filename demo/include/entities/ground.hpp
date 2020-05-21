#ifndef _GROUND_HPP_
#define _GROUND_HPP_

#include "collisionComponentInterface.hpp"
#include "renderableInterface.hpp"

#include "ground.hpp"

namespace Entity
{
    class Ground : public Renderer::RenderableInterface,
                   public Physics::CollisionComponentInterface<Box>
    {
    private:

    public:
        Physics::GTransform transform;

        Ground() 
            : Renderer::RenderableInterface(&transform),
              Physics::CollisionComponentInterface<Box>(&transform)
        {
            // collider.transform = &transform;
        }

        void setResources(const DemoResourceManager&);
    };
}

#endif