#ifndef _BASIC_ENTITY_HPP_
#define _BASIC_ENTITY_HPP_

#include "entityID.hpp"

#include "box.hpp"
#include "saveInterface.hpp"
#include "collisionComponentInterface.hpp"
#include "physicComponentInterface.hpp"

#include "mesh.hpp"
#include "rendererSystem.hpp"

namespace Entity
{
    // Example Class for rendering an object
    class BasicEntity : public EntityID, 
                        public Physics::CollisionComponentInterface<Box>, 
                        public Physics::PhysicComponentInterface, 
                        public Save::SaveInterface
    {
    public:
        Physics::GTransform* transform = nullptr;

        Renderer::MeshIt meshIt;
        Renderer::Mesh mesh;
        // Physics::PhysicComponent physicComponent; // moving sphere 

        // to load graphs
        Physics::GraphKey key;

        bool isEnabled = true;

    public:

        BasicEntity() = default;
        inline BasicEntity(const EntityID& id)
            : EntityID(id)
        {

        }
        ~BasicEntity()
        {
            if (transform != nullptr)
            {
                delete transform;
            }
        }

        void setup(Renderer::RendererSystem& renderer, 
                    const Resources::Model* model, 
                    const Resources::Shader* shader, 
                    Physics::TransformGraph& transformParent);

        virtual void raycastCollide();

        void setup(Renderer::RendererSystem& renderer, 
                    const Resources::Model* model, 
                    const Resources::Shader* shader,
                    const Resources::Texture* texture,
                    Physics::TransformGraph& transformParent);

        // save any data you want with this function
        void save(Save::Saver& saver) override;
        // loads the data loaded from the save file
        void loadData(Save::Loader& loader) override;
        // after loading data, this function will be called to set pointers, iterators, references...
        void loadLinks(Physics::TransformGraph& root);

        virtual void physicCompOnCollisionEnter        (const SegmentHit&) override {}
        virtual void physicCompOnCollisionExit         () override {}
        virtual void physicCompOnOverlapEnter   (const Physics::PhysicsSystem::CollisionsCallbacksSentData& data) override
        {

        }

        virtual void colliderOnCollisionEnter (const SegmentHit&) override {};
        virtual void colliderOnCollisionExit  () override {};
        virtual void colliderOnOverlapEnter   (const Physics::PhysicsSystem::CollisionsCallbacksSentData& data) override
        {

        }
    };
}

#endif