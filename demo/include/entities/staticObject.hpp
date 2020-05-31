#ifndef _STATIC_OBJECT_HPP_
#define _STATIC_OBJECT_HPP_

#include "collisionComponentInterface.hpp"
#include "renderableInterface.hpp"
#include "transformInterface.hpp"

#include  "demoResourceManager.hpp"

namespace Entity
{
    class StaticObject : public Physics::TransformInterface,
                         public Renderer::RenderableInterface,
                         public Physics::CollisionComponentInterface<Physics::Shapes::Box>
    {
    public:
        StaticObject() 
            : Renderer::RenderableInterface(&transform),
              Physics::CollisionComponentInterface<Physics::Shapes::Box>(&transform)
        {

        }

        StaticObject(const Core::Maths::Vec4& color) 
            : Renderer::RenderableInterface(&transform),
              Physics::CollisionComponentInterface<Physics::Shapes::Box>(&transform)
        {

        }


        void setResources(const DemoResourceManager&,                                   
                          E_Model model, 
                          E_Shader shader,
                          E_Texture texture);
    };
}

#endif