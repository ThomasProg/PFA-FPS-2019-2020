#include "rendererSystem.hpp"

constexpr size_t Renderer::RendererSystem::maxChildrenAtLoad;

// Renderer::RendererSystem::iterator Renderer::RendererSystem::addComponentTo(Entity::EntityID& entity)
// {
//     meshes.emplace(entity, Mesh());
//     iterator it {entity, this};

//     return it;
// }

Renderer::RendererSystem::iterator Renderer::RendererSystem::addComponentTo(Renderer::Mesh* mesh)
{
    meshes.emplace_back(mesh);
    return iterator{meshes.size() - 1};
}

Renderer::RendererSystem::iterator Renderer::RendererSystem::erase(Renderer::RendererSystem::iterator& it)
{
    assert(it < meshes.size());
    meshes[it] = nullptr;
    return it++;
}

void Renderer::RendererSystem::draw(const Camera& camera, Renderer::LightManager& lightManager)
{
    for (Mesh* mesh : meshes)
    {
        if (mesh != nullptr)
        {
            mesh->draw(camera, lightManager);
        }
    }
}


void Renderer::RendererSystem::save(Save::Saver& saver) 
{
    // saver.save(meshes.size());
    // // for (std::pair<const Entity::EntityID, Mesh> pair : meshes)
    // for (Renderer::Mesh* mesh : meshes)
    // {
    //     saver.save(pair.first);
    //     saver.save(pair.second.transform->transform);
    //     saver.save(pair.second.isDrawn);
    // }
}

void Renderer::RendererSystem::loadData(Save::Loader& loader) 
{
    // size_t nbMeshes;
    // loader.load(nbMeshes);
    // nbMeshes = std::min(nbMeshes, maxChildrenAtLoad);
    // for (size_t i = 0; i < nbMeshes; i++)
    // {
    //     std::pair<Entity::EntityID, Mesh> pair;
    //     loader.load(pair.first);
    //     loader.load(pair.second.transform->transform);
    //     loader.load(pair.second.isDrawn);
    //     meshes.emplace(std::move(pair));
    // }

    // loader.tryToDisplayError(__FILE__);
}
