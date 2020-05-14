#include "rendererSystem.hpp"

#include "renderDebug.hpp"

constexpr size_t Renderer::RendererSystem::maxChildrenAtLoad;

Renderer::RendererSystem::iterator Renderer::RendererSystem::addComponentTo(Entity::Entity& entity)
{
    auto rit = meshes.emplace(entity, Mesh());
    iterator it {entity, this};

    return it;
}

void Renderer::RendererSystem::draw(const Camera& camera)
{
    for (std::pair<const Entity::Entity, Mesh>& pair : meshes)
    {
        pair.second.draw(camera);
    }
}


void Renderer::RendererSystem::save(Save::Saver& saver) 
{
    saver.save(meshes.size());
    for (std::pair<const Entity::Entity, Mesh> pair : meshes)
    {
        saver.save(pair.first);
        saver.save(pair.second.transform.transform);
        saver.save(pair.second.isDrawn);
    }
}

void Renderer::RendererSystem::loadData(Save::Loader& loader) 
{
    size_t nbMeshes;
    loader.load(nbMeshes);
    nbMeshes = std::min(nbMeshes, maxChildrenAtLoad);
    for (size_t i = 0; i < nbMeshes; i++)
    {
        std::pair<Entity::Entity, Mesh> pair;
        loader.load(pair.first);
        loader.load(pair.second.transform.transform);
        loader.load(pair.second.isDrawn);
        meshes.emplace(std::move(pair));
    }

    loader.tryToDisplayError(__FILE__);
}
