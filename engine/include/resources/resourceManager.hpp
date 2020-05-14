#ifndef _RESOURCE_MANAGER_HPP_
#define _RESOURCE_MANAGER_HPP_

#include "model.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include <unordered_map>

namespace Resources
{
    template<typename CAPACITY_TYPE = unsigned int>
    class ResourceManager
    {
    public:
        using ContainerCapacityType = CAPACITY_TYPE;
        using Type = ResourceManager<CAPACITY_TYPE>;

    protected:
        std::unordered_map<ContainerCapacityType, Model> models;
        std::unordered_map<ContainerCapacityType, Texture> textures;
        std::unordered_map<ContainerCapacityType, Shader> shaders;

    public:
        // Returns const elements
        inline const Model&   getModel(ContainerCapacityType   id) const;
        inline const Texture& getTexture(ContainerCapacityType id) const;
        inline const Shader&  getShader(ContainerCapacityType  id) const;

        // Only available per move
        inline void add(Shader&& shader,   ContainerCapacityType key);
        inline void add(Model&& model,     ContainerCapacityType key);
        inline void add(Texture&& texture, ContainerCapacityType key);
    };
}

#include "resourceManager.inl"

#endif