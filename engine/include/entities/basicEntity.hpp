#ifndef _BASIC_ENTITY_HPP_
#define _BASIC_ENTITY_HPP_

#include "entity.hpp"
#include "rendererSystem.hpp"
#include "physicsSystem.hpp"
#include "resourceManager.hpp"

#include "box.hpp"
#include "saveInterface.hpp"

namespace Entity
{
    // Example Class for rendering an object
    class BasicEntity : public Entity, public Save::SaveInterface
    {
    public:
        Physics::GraphKey key;
        Renderer::MeshIt mesh;
        Physics::PhysicComponent physicComponent; // moving sphere 
        Physics::ColliderIt colliderCompo; // box
        bool isEnabled = true;

    public:

        void setup(Renderer::RendererSystem& renderer, 
                    const Resources::Model* model, 
                    const Resources::Shader* shader, 
                    Physics::TransformGraph& transformParent) 
        {
            if (!mesh.isValid())
                mesh = renderer.addComponentTo(*this);

            // transform 
            mesh->transform.transformMatrixNode = transformParent.addChild();

            // resources
            mesh->model  = model;
            mesh->shader = shader;
            mesh->linkShaderWithModel();
        }

        virtual void raycastCollide();

        void setup(Renderer::RendererSystem& renderer, 
                    const Resources::Model* model, 
                    const Resources::Shader* shader,
                    const Resources::Texture* texture,
                    Physics::TransformGraph& transformParent) 
        {
            if (!mesh.isValid())
                mesh = renderer.addComponentTo(*this);

            // transform 
            mesh->transform.transformMatrixNode = transformParent.addChild();

            // resources
            mesh->model  = model;
            mesh->shader = shader;
            mesh->texture = texture;
            mesh->linkShaderWithModel();
        }

        // save any data you want with this function
        void save(Save::Saver& saver) override;
        // loads the data loaded from the save file
        void loadData(Save::Loader& loader) override;
        // after loading data, this function will be called to set pointers, iterators, references...
        void loadLinks(Physics::TransformGraph& root);
    };
}

#endif