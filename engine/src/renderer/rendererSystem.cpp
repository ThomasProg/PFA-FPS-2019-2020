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

                // for (uint i = 0; i < lightManager.lights.size(); i++)
                // {
                //     // light.lightData.location = Core::Maths::Vec4{cam.transform.transformMatrixNode->worldData.getTranslationVector(), 1};
                //     lastShader->linkLight(i, lightManager.lights[i].lightData, lightManager.lightsUniformBuffer);
                // }
                // lastShader->linkLight2(lightManager);
            }
            
            mesh->draw(camera, lightManager);
        }
    }

    lightManager.hasLightChanged = false;
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
