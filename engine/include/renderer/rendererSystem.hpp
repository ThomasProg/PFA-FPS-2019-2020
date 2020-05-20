#ifndef _RENDERER_SYSTEM_HPP_
#define _RENDERER_SYSTEM_HPP_

#include <unordered_map>

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
        // Represents an Index.
        // This means an index can't take more size that this type's size.
        // Used to erase the element from the sytem.
        using iterator = unsigned int;

    private:
        std::vector<Mesh*> meshes;
        std::vector<iterator> freeMeshIndices;

    public:

        RendererSystem() = default;

        // Adds the mesh pointed by the pointer to the render list.
        //
        // Inputs : A pointer towards a valid mesh (not nullptr).
        // Outputs : The iterator corresponding to the stored mesh pointer.
        //
        // WARNING : 
        // - The mesh should be static in memory while being in the render list. 
        //   If this requirement is not met, this results in undefined behavior.
        // - No order is guaranteed.
        //
        // Speed :
        // Average Case : O(1)
        // Worst Case : O(n), n being meshes.size()
        iterator addComponent(Renderer::Mesh* mesh);

        // Removes the reference to the mesh corresponding to the iterator.
        // 
        // Inputs : The corresponding iterator.
        //
        // WARNING : 
        // - The iterator should be valid.
        //   A valid iterator is an iterator returned by addComponent(), 
        //   which has not been erased beforehand.
        //
        // Speed : 
        // Average Case : O(1)
        // Worst Case : O(n), n being freeMeshIndices.size()
        void erase(iterator& it);

        void draw(const Camera& camera, Renderer::LightManager& lightManager);

        void save(Save::Saver& saver) override;
        void loadData(Save::Loader& loader) override;

        void reset()
        {
            meshes.clear();
            freeMeshIndices.clear();
        }

        //friend iterator;
    };

    // MeshComponents are used by the RendererSystem.
    using MeshIt = RendererSystem::iterator;//std::unordered_map<Entity::Entity, Mesh>::iterator;
}

#endif