#ifndef _RENDERED_ENTITY_HPP_
#define _RENDERED_ENTITY_HPP_

#include "rendererSystem.hpp"

namespace Entity
{
    class RenderedEntity//: public EntityID
    {
    private:
        
    public:
        Physics::GTransform transform;
        Physics::GraphKey key;

        Renderer::MeshIt meshIt;
        Renderer::Mesh mesh {&transform};

        float lifeTime = 2.0f;
        float timer;
        
        RenderedEntity() = default;
        // RenderedEntity(const RenderedEntity& rhs)
        // {
        //     if (rhs.transform != nullptr)
        //     {
        //         transform = new Physics::GTransform();
        //         *transform = *rhs.transform;
        //     }
        //     key = rhs.key;
        //     mesh = rhs.mesh;
        //     lifeTime = rhs.lifeTime;
        //     timer = rhs.timer;
        //     // entityID = rhs.entityID;
        // } 
        // RenderedEntity(RenderedEntity&& rhs) noexcept
        // {
        //     if (rhs.transform != nullptr)
        //     {
        //         transform = rhs.transform;
        //         rhs.transform = nullptr;
        //     }
        //     key = rhs.key;
        //     mesh = rhs.mesh;
        //     lifeTime = rhs.lifeTime;
        //     timer = rhs.timer;
        //     // entityID = rhs.entityID;
        // }

        // RenderedEntity& operator=(const RenderedEntity& rhs)
        // {
        //     if (rhs.transform != nullptr)
        //     {
        //         transform = new Physics::GTransform();
        //         *transform = *rhs.transform;
        //     }
        //     key = rhs.key;
        //     mesh = rhs.mesh;
        //     lifeTime = rhs.lifeTime;
        //     timer = rhs.timer;
        //     // entityID = rhs.entityID;
        //     return *this;
        // }

        // RenderedEntity& operator=(RenderedEntity&& rhs)
        // {
        //     if (rhs.transform != nullptr)
        //     {
        //         transform = rhs.transform;
        //         rhs.transform = nullptr;
        //     }
        //     key = rhs.key;
        //     mesh = rhs.mesh;
        //     lifeTime = rhs.lifeTime;
        //     timer = rhs.timer;
        //     // entityID = rhs.entityID;
        //     return *this;
        // }

        // // inline RenderedEntity(const EntityID& id)
        // //     : EntityID(id)
        // // {

        // // }
        
        void setup(Renderer::RendererSystem& renderer, 
                    const Resources::Model* model, 
                    const Resources::Shader* shader, 
                    Physics::TransformGraph& transformParent);

        void setup(Renderer::RendererSystem& renderer, 
                    const Resources::Model* model, 
                    const Resources::Shader* shader,
                    const Resources::Texture* texture,
                    Physics::TransformGraph& transformParent);
        
        void setTransform(const Physics::Transform& newTransform)
        {
            this->transform.transform = newTransform;
            transform.UpdateLocalTransformMatrix();
            transform.transformMatrixNode->setDirtySelfAndChildren();
        }

        ~RenderedEntity() = default;
    };
}

#endif