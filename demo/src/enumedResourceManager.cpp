#include "enumedResourceManager.hpp"

const Resources::Model& EnumedResourceManager::get(E_Model modelID) const
{
    return Resources::ResourceManager<ContainerCapacityType>::getModel(static_cast<ContainerCapacityType>(modelID));
}

const Resources::Texture& EnumedResourceManager::get(E_Texture textureID) const
{
    return Resources::ResourceManager<ContainerCapacityType>::getTexture(static_cast<ContainerCapacityType>(textureID));
}

const Resources::Shader& EnumedResourceManager::get(E_Shader shaderID) const
{
    return Resources::ResourceManager<ContainerCapacityType>::getShader(static_cast<ContainerCapacityType>(shaderID));
}

void EnumedResourceManager::add(Resources::Model&& model, E_Model modelID)
{
    Resources::ResourceManager<ContainerCapacityType>::add(std::move(model), static_cast<ContainerCapacityType>(modelID));
}

void EnumedResourceManager::add(Resources::Shader&& shader, E_Shader shaderID)
{
    Resources::ResourceManager<ContainerCapacityType>::add(std::move(shader), static_cast<ContainerCapacityType>(shaderID));
}

void EnumedResourceManager::add(Resources::Texture&& texture, E_Texture textureID)
{
    Resources::ResourceManager<ContainerCapacityType>::add(std::move(texture), static_cast<ContainerCapacityType>(textureID));
}
