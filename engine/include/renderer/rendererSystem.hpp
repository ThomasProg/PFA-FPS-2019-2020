#ifndef _RENDERER_SYSTEM_HPP_
#define _RENDERER_SYSTEM_HPP_

#include <unordered_map>

#include "entity.hpp"
#include "mesh.hpp"

#include "saveInterface.hpp"

namespace Renderer
{   
    // We have a system for some reasons :
    // - we want to sort meshes in a specific order before rendering 
    // - because of that, we have to access to all the meshes 
    class RendererSystem : public Save::SaveInterface
    {
    private:
        static constexpr size_t maxChildrenAtLoad = 100;

    public:
        class iterator
        {
        private:
            Entity::Entity entityID;
            RendererSystem* rendererSystem = nullptr;

        public:
            iterator()
            {
                
            }

            iterator(Entity::Entity& entityID, RendererSystem* rendererSystem)
                : entityID(entityID), rendererSystem(rendererSystem)
            {

            }

            Mesh* operator->()
            {
                assert(rendererSystem != nullptr);
                return &rendererSystem->meshes.at(entityID);
            }

            const Mesh* operator->() const
            {
                assert(rendererSystem != nullptr);
                return &rendererSystem->meshes.at(entityID);
            }

            bool isValid() const noexcept
            {
                return rendererSystem != nullptr;
            }

            friend RendererSystem;
        };

    private:
        std::unordered_map<Entity::Entity, Mesh> meshes;

    public:
        RendererSystem() = default;

        iterator addComponentTo(Entity::Entity& entity);

        void remove(decltype(meshes)::iterator& it);

        void draw(const Camera& camera);

        void save(Save::Saver& saver) override;
        void loadData(Save::Loader& loader) override;

        friend iterator;
    };

    // MeshComponents are used by the RendererSystem.
    using MeshIt = RendererSystem::iterator;//std::unordered_map<Entity::Entity, Mesh>::iterator;
}

#endif