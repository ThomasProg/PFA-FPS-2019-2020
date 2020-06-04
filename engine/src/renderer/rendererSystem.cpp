#include "rendererSystem.hpp"
#include "lightManager.hpp"

#include "shader.hpp"

constexpr size_t Renderer::RendererSystem::maxChildrenAtLoad;

Renderer::RendererSystem::iterator Renderer::RendererSystem::addComponent(Renderer::Mesh* mesh)
{
    assert(mesh != nullptr);
    if (freeMeshIndices.empty())
    {
        meshes.emplace_back(mesh);
        return iterator{((unsigned int) meshes.size()) - 1u};
    }
    else 
    {
        iterator newIt = freeMeshIndices.back(); 
        meshes[newIt] = mesh;
        // Always constant, compared to erasing the first element.
        freeMeshIndices.pop_back(); 
        return newIt;
    }
}

void Renderer::RendererSystem::erase(Renderer::RendererSystem::iterator& it)
{
    // We check if the index is valid (inside the container).
    // It it valid, that means there is a last element, 
    // that we can get with back(), and that we can remove with pop_back().
    assert(it < meshes.size());
    meshes[it] = nullptr;
    freeMeshIndices.emplace_back(it);
}

void Renderer::RendererSystem::draw(const Camera& camera, Renderer::LightManager& lightManager)
{
    const Resources::Shader* lastShader = nullptr;
    for (Mesh* mesh : meshes)
    {
        if (mesh != nullptr)
        {
            if (mesh->shader != nullptr && mesh->shader != lastShader)
            {
                mesh->shader->use();
                lastShader = mesh->shader;
                
                if (lightManager.hasLightChanged)
                    lastShader->useLightsUniformValues(lightManager);
            }
            
            mesh->draw(camera, lightManager);
        }
    }

    lightManager.hasLightChanged = false;
}