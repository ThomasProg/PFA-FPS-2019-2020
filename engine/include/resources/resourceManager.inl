#include "resourceManager.hpp"

template<typename CAPACITY_TYPE>
inline const Resources::Model& Resources::ResourceManager<CAPACITY_TYPE>::getModel(ContainerCapacityType id) const
{
    assert(models.count(id) == 1);
    return models.at(id);
}

template<typename CAPACITY_TYPE>
inline const Resources::Shader& Resources::ResourceManager<CAPACITY_TYPE>::getShader(ContainerCapacityType id) const
{
    assert(shaders.count(id) == 1);
    return shaders.at(id);
}

template<typename CAPACITY_TYPE>
inline const Resources::Texture& Resources::ResourceManager<CAPACITY_TYPE>::getTexture(ContainerCapacityType id) const
{
    assert(textures.count(id) == 1);
    return textures.at(id);
}



template<typename CAPACITY_TYPE>
inline void Resources::ResourceManager<CAPACITY_TYPE>::add(Resources::Model&& model, ContainerCapacityType key)
{
    models.emplace(std::make_pair(key, std::move(model)));
}

template<typename CAPACITY_TYPE>
inline void Resources::ResourceManager<CAPACITY_TYPE>::add(Resources::Shader&& shader, ContainerCapacityType key)
{
    shaders.emplace(std::make_pair(key, std::move(shader)));
}

template<typename CAPACITY_TYPE>
inline void Resources::ResourceManager<CAPACITY_TYPE>::add(Resources::Texture&& texture, ContainerCapacityType key)
{
    textures.emplace(std::make_pair(key, std::move(texture)));
}

