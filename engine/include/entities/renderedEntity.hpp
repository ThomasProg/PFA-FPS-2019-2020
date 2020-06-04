#ifndef _RENDERED_ENTITY_HPP_
#define _RENDERED_ENTITY_HPP_

#include "demoResourceManager.hpp"

#include "transformInterface.hpp"
#include "renderableInterface.hpp"

namespace Entity
{
    class RenderedEntity : public Physics::TransformInterface,
                           public Renderer::RenderableInterface
    {
    public:

        float lifeTime = 2.0f;
        float timer;
        
        RenderedEntity()
            : RenderableInterface(&transform)
        {

        }

        void setResources(const DemoResourceManager&);

        ~RenderedEntity() = default;
    };
}

#endif