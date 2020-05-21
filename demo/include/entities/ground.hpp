#ifndef _GROUND_HPP_
#define _GROUND_HPP_

#include "collisionComponentInterface.hpp"
#include "renderableInterface.hpp"
#include "transformInterface.hpp"

#include "ground.hpp"

#include  "demoResourceManager.hpp"

namespace Entity
{
    class Ground : public Physics::TransformInterface,
                   public Renderer::RenderableInterface,
                   public Physics::CollisionComponentInterface<Box>
    {
    public:
        Ground() 
            : Renderer::RenderableInterface(&transform),
              Physics::CollisionComponentInterface<Box>(&transform)
        {

        }

        void setResources(const DemoResourceManager&);
    };
}

#endif