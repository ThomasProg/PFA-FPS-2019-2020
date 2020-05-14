#ifndef _ENUMED_RESOURCE_MANAGER_HPP_
#define _ENUMED_RESOURCE_MANAGER_HPP_

#include "resourceManager.hpp"

class EnumedResourceManager : public Resources::ResourceManager<unsigned char>
{
public:

    using ContainerCapacityType = Type::ContainerCapacityType;

    enum class E_Model : EnumedResourceManager::ContainerCapacityType
    {
        E_SPHERE = 0,
        E_BOX,
        E_DOG,
    };

    enum class E_Texture : EnumedResourceManager::ContainerCapacityType
    {
        E_DOG_TEXTURE = 0,
        E_GROUND
    };

    enum class E_Shader : EnumedResourceManager::ContainerCapacityType
    {
        E_FLAT = 0,
        E_TEXTURED,
    };

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

using E_Model   = EnumedResourceManager::E_Model;
using E_Shader  = EnumedResourceManager::E_Shader;
using E_Texture = EnumedResourceManager::E_Texture;

#endif 