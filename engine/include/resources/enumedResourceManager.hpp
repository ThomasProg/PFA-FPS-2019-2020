#ifndef _ENUMED_RESOURCE_MANAGER_HPP_
#define _ENUMED_RESOURCE_MANAGER_HPP_

#include "resourceManager.hpp"

template<typename MODEL_ENUM, typename TEXTURE_ENUM, typename SHADER_ENUM, typename CAPACITY_TYPE = unsigned int>
class EnumedResourceManager : public Resources::ResourceManager<CAPACITY_TYPE>
{
    static_assert(std::is_enum<MODEL_ENUM>::value);
    static_assert(std::is_enum<TEXTURE_ENUM>::value);
    static_assert(std::is_enum<SHADER_ENUM>::value);

public:
    using ContainerCapacityType = CAPACITY_TYPE;

    using E_Model   = MODEL_ENUM;
    using E_Texture = TEXTURE_ENUM;
    using E_Shader  = SHADER_ENUM;

private:
    // Returns const elements
    const Resources::Model&   getModel(unsigned int modelID)   const;
    const Resources::Shader&  getShader(unsigned int modelID)  const;
    const Resources::Texture& getTexture(unsigned int modelID) const;

    // Only available per move
    void add(Resources::Model&& model,     unsigned int key);
    void add(Resources::Shader&& shader,   unsigned int key);
    void add(Resources::Texture&& texture, unsigned int key);

public:
    const Resources::Model&   get(E_Model   modelID)   const;
    const Resources::Shader&  get(E_Shader  shaderID)  const;
    const Resources::Texture& get(E_Texture textureID) const;

    void add(Resources::Model&& model,     E_Model   modelID);
    void add(Resources::Shader&& shader,   E_Shader  shaderID);
    void add(Resources::Texture&& texture, E_Texture textureID);
};

#include "enumedResourceManager.inl"

#endif 