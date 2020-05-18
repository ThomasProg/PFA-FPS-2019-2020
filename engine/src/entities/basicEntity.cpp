#include "basicEntity.hpp"

#include "loader.hpp"
#include "saver.hpp"

// save any data you want with this function
void Entity::BasicEntity::save(Save::Saver& saver)
{
    // Entity
    saver.save(entityID);

    // Renderer
    bool isMeshValid = mesh.isValid() && mesh->transform.transformMatrixNode.isValid();
    saver.save(isMeshValid);
    if (isMeshValid)
        saver.save(mesh->transform.transformMatrixNode->graphID);

    // Physics
    bool isPhysicCompValid = physicCompIt.isValid();
    saver.save(isPhysicCompValid); 
    if (isPhysicCompValid)
        saver.save(physicCompIt->velocity);
}

// loads the data loaded from the save file
void Entity::BasicEntity::loadData(Save::Loader& loader)
{
    // Entity
    loader.load(entityID);

    // Renderer
    bool isMeshValid;
    loader.load(isMeshValid);
    if (isMeshValid)
        loader.load(key);

    // // Physics
    // bool isPhysicCompValid;
    // loader.load(isPhysicCompValid);
    // if (isPhysicCompValid)
    //     loader.load(physicCompIt->velocity);

    loader.tryToDisplayError(__FILE__);
}

// after loading data, this function will be called to set pointers, iterators, references...
void Entity::BasicEntity::loadLinks(Physics::TransformGraph& root)
{
    if (!(key == 0))
    {
        root.foreach([&](Physics::TransformGraph& g)
        {
            if (g.graphID == key && mesh.isValid())
            {
                if (mesh->transform.transformMatrixNode.isValid())
                {
                    mesh->transform.transformMatrixNode.erase();
                }
                mesh->transform.transformMatrixNode = Physics::TransformGraph::iterator{&g};
            }
        });
    }
}

void Entity::BasicEntity::raycastCollide()
{
    
}

void Entity::BasicEntity::setup(Renderer::RendererSystem& renderer, 
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

void Entity::BasicEntity::setup(Renderer::RendererSystem& renderer, 
            const Resources::Model* model, 
            const Resources::Shader* shader,
            const Resources::Texture* texture,
            Physics::TransformGraph& transformParent) 
{
    setup(renderer, model, shader, transformParent);
    mesh->texture = texture;
}
    