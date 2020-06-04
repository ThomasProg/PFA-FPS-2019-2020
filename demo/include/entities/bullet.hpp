#ifndef _BULLET_HPP_
#define _BULLET_HPP_

#include "demoResourceManager.hpp"

#include "transformInterface.hpp"
#include "renderableInterface.hpp"

namespace Entity
{
    class Bullet : public Physics::TransformInterface,
                   public Renderer::RenderableInterface
    {
    public:

        float lifeTime = 2.0f;
        float timer;
        
        Bullet()
            : RenderableInterface(&transform)
        {
            mesh.color   = {0.988f, 1.f, 0.478f, 1};
        }

        void setResources(const DemoResourceManager&);

        ~Bullet() = default;
    };
}

#endif