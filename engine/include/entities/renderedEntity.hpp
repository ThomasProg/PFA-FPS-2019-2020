#ifndef _RENDERED_ENTITY_HPP_
#define _RENDERED_ENTITY_HPP_

#include "entityID.hpp"
#include "rendererSystem.hpp"

namespace Entity
{
    class RenderedEntity: public EntityID
    {
    private:
        
    public:
        Physics::GraphKey key;
        Renderer::MeshIt mesh;
        float lifeTime = 2.0f;
        float timer;
        
        RenderedEntity() = default;

        inline RenderedEntity(const EntityID& id)
            : EntityID(id)
        {

        }
        
        void setup(Renderer::RendererSystem& renderer, 
                    const Resources::Model* model, 
                    const Resources::Shader* shader, 
                    Physics::TransformGraph& transformParent);

        void setup(Renderer::RendererSystem& renderer, 
                    const Resources::Model* model, 
                    const Resources::Shader* shader,
                    const Resources::Texture* texture,
                    Physics::TransformGraph& transformParent);
        

        ~RenderedEntity() = default;
    };
}

#endif