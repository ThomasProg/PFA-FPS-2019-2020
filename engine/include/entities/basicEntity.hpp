#ifndef _BASIC_ENTITY_HPP_
#define _BASIC_ENTITY_HPP_

#include "box.hpp"

#include "saveInterface.hpp"
#include "collisionComponentInterface.hpp"
#include "physicComponentInterface.hpp"
#include "renderableInterface.hpp"
#include "transformInterface.hpp"

#include "mesh.hpp"
#include "rendererSystem.hpp"

// forward declarations
template<typename, typename, typename, typename>
class EnumedResourceManager;
using DemoResourceManager = EnumedResourceManager<E_Model, E_Texture, E_Shader, unsigned int>;

namespace Entity
{
    // Example Class for rendering an object
    class BasicEntity : public Physics::TransformInterface,
                        public Physics::CollisionComponentInterface<Box>, 
                        public Physics::PhysicComponentInterface, 
                        public Renderer::RenderableInterface,
                        public Save::SaveInterface
    {
    public:
        // Physics::GTransform transform;

        // Physics::PhysicComponent physicComponent; // moving sphere 

        // to load graphs
        // Physics::GraphKey key;

    public:

        BasicEntity() 
        {
            collider.transform = physicComp.collider.transform = mesh.transform = &transform;
        }
        // inline BasicEntity(const EntityID& id)
        //     : EntityID(id)
        // {

        // }
        ~BasicEntity() = default;

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

        void setResources(const DemoResourceManager&);

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