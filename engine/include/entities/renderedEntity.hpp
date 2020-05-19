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
        Physics::GTransform* transform = nullptr;
        Physics::GraphKey key;
        Renderer::MeshIt mesh;
        float lifeTime = 2.0f;
        float timer;
        
        RenderedEntity() = default;
        RenderedEntity(const RenderedEntity& rhs)
        {
            if (rhs.transform != nullptr)
            {
                transform = new Physics::GTransform();
                *transform = *rhs.transform;
            }
            key = rhs.key;
            mesh = rhs.mesh;
            lifeTime = rhs.lifeTime;
            timer = rhs.timer;
            entityID = rhs.entityID;
        } 
        RenderedEntity(RenderedEntity&& rhs) noexcept
        {
            if (rhs.transform != nullptr)
            {
                transform = rhs.transform;
                rhs.transform = nullptr;
            }
            key = rhs.key;
            mesh = rhs.mesh;
            lifeTime = rhs.lifeTime;
            timer = rhs.timer;
            entityID = rhs.entityID;
        }

        RenderedEntity& operator=(const RenderedEntity& rhs)
        {
            if (rhs.transform != nullptr)
            {
                transform = new Physics::GTransform();
                *transform = *rhs.transform;
            }
            key = rhs.key;
            mesh = rhs.mesh;
            lifeTime = rhs.lifeTime;
            timer = rhs.timer;
            entityID = rhs.entityID;
            return *this;
        }

        RenderedEntity& operator=(RenderedEntity&& rhs)
        {
            if (rhs.transform != nullptr)
            {
                transform = rhs.transform;
                rhs.transform = nullptr;
            }
            key = rhs.key;
            mesh = rhs.mesh;
            lifeTime = rhs.lifeTime;
            timer = rhs.timer;
            entityID = rhs.entityID;
            return *this;
        }

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
        

        ~RenderedEntity() 
        {
            if (transform != nullptr)
            {
                delete transform;
            }
        }
    };
}

#endif