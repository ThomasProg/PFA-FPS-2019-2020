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
    saver.save(physicComponent.velocity);
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

    // Physics
    loader.load(physicComponent.velocity);

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
