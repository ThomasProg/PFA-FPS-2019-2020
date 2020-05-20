#ifndef _RENDERER_SYSTEM_HPP_
#define _RENDERER_SYSTEM_HPP_

#include <unordered_map>

#include "entityID.hpp"
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
        // class iterator
        // {
        // private:
        //     unsigned int elementIndex = 0;

        // public:
        //     iterator() = default;

        //     iterator(unsigned int elementIndex) 
        //         : elementIndex(elementIndex)
        //     {

        //     }

        // //     Entity::EntityID entityID;
        // //     RendererSystem* rendererSystem = nullptr;

        // // public:
        // //     iterator() = default;

        // //     iterator(Entity::EntityID& entityID, RendererSystem* rendererSystem)
        // //         : entityID(entityID), rendererSystem(rendererSystem)
        // //     {

        // //     }

        // //     Mesh* operator->()
        // //     {
        // //         assert(rendererSystem != nullptr);
        // //         return &rendererSystem->meshes.at(entityID);
        // //     }

        // //     const Mesh* operator->() const
        // //     {
        // //         assert(rendererSystem != nullptr);
        // //         return &rendererSystem->meshes.at(entityID);
        // //     }

        // //     bool isValid() const noexcept
        // //     {
        // //         return rendererSystem != nullptr;
        // //     }

        // //     void erase()
        // //     {
        // //         assert(rendererSystem != nullptr);
        // //         rendererSystem->meshes.erase(entityID);
        // //     }

        //     friend RendererSystem;
        // };
        using iterator = unsigned int;

    private:
        // std::unordered_map<Entity::EntityID, Mesh> meshes;
        std::vector<Mesh*> meshes;

    public:
        RendererSystem() = default;

        // iterator addComponentTo(Entity::EntityID& entity);
        iterator addComponentTo(Renderer::Mesh* mesh);

        // returns an iterator to the next element
        iterator erase(iterator& it);

        void draw(const Camera& camera, Renderer::LightManager& lightManager);

        void save(Save::Saver& saver) override;
        void loadData(Save::Loader& loader) override;

        void reset()
        {
            meshes.clear();
        }

        //friend iterator;
    };

    // MeshComponents are used by the RendererSystem.
    using MeshIt = RendererSystem::iterator;//std::unordered_map<Entity::Entity, Mesh>::iterator;
}

#endif