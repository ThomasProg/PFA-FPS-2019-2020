#include "enumedResourceManager.hpp"

template<typename MODEL_ENUM, typename TEXTURE_ENUM, typename SHADER_ENUM, typename CAPACITY_TYPE>
const Resources::Model& EnumedResourceManager<MODEL_ENUM, TEXTURE_ENUM, SHADER_ENUM, CAPACITY_TYPE>::get(E_Model modelID) const
{
    return Resources::ResourceManager<ContainerCapacityType>::getModel(static_cast<ContainerCapacityType>(modelID));
}

template<typename MODEL_ENUM, typename TEXTURE_ENUM, typename SHADER_ENUM, typename CAPACITY_TYPE>
const Resources::Texture& EnumedResourceManager<MODEL_ENUM, TEXTURE_ENUM, SHADER_ENUM, CAPACITY_TYPE>::get(E_Texture textureID) const
{
    return Resources::ResourceManager<ContainerCapacityType>::getTexture(static_cast<ContainerCapacityType>(textureID));
}

template<typename MODEL_ENUM, typename TEXTURE_ENUM, typename SHADER_ENUM, typename CAPACITY_TYPE>
const Resources::Shader& EnumedResourceManager<MODEL_ENUM, TEXTURE_ENUM, SHADER_ENUM, CAPACITY_TYPE>::get(E_Shader shaderID) const
{
    return Resources::ResourceManager<ContainerCapacityType>::getShader(static_cast<ContainerCapacityType>(shaderID));
}

template<typename MODEL_ENUM, typename TEXTURE_ENUM, typename SHADER_ENUM, typename CAPACITY_TYPE>
void EnumedResourceManager<MODEL_ENUM, TEXTURE_ENUM, SHADER_ENUM, CAPACITY_TYPE>::add(Resources::Model&& model, E_Model modelID)
{
    Resources::ResourceManager<ContainerCapacityType>::add(std::move(model), static_cast<ContainerCapacityType>(modelID));
}

template<typename MODEL_ENUM, typename TEXTURE_ENUM, typename SHADER_ENUM, typename CAPACITY_TYPE>
void EnumedResourceManager<MODEL_ENUM, TEXTURE_ENUM, SHADER_ENUM, CAPACITY_TYPE>::add(Resources::Shader&& shader, E_Shader shaderID)
{
    Resources::ResourceManager<ContainerCapacityType>::add(std::move(shader), static_cast<ContainerCapacityType>(shaderID));
}

template<typename MODEL_ENUM, typename TEXTURE_ENUM, typename SHADER_ENUM, typename CAPACITY_TYPE>
void EnumedResourceManager<MODEL_ENUM, TEXTURE_ENUM, SHADER_ENUM, CAPACITY_TYPE>::add(Resources::Texture&& texture, E_Texture textureID)
{
    Resources::ResourceManager<ContainerCapacityType>::add(std::move(texture), static_cast<ContainerCapacityType>(textureID));
}