#ifndef _DEMO_RESOURCE_MANAGER_HPP_
#define _DEMO_RESOURCE_MANAGER_HPP_

#include "enumedResourceManager.hpp"

// User custom container type :
using DemoResourceManagerContainerType = unsigned int;

// Forward Declarations :
enum class E_Model   : DemoResourceManagerContainerType;
enum class E_Texture : DemoResourceManagerContainerType;
enum class E_Shader  : DemoResourceManagerContainerType;

// User custom typename
using DemoResourceManager = EnumedResourceManager<E_Model, E_Texture, E_Shader, DemoResourceManagerContainerType>;

// User custom Enums :
enum class E_Model : DemoResourceManager::ContainerCapacityType
{
    E_SPHERE = 0,
    E_BOX,
    E_DOG,
    E_GUN,
    E_TREE,
};

enum class E_Texture : DemoResourceManager::ContainerCapacityType
{
    E_DOG_TEXTURE = 0,
    E_GROUND,
    E_GUN,
};

enum class E_Shader : DemoResourceManager::ContainerCapacityType
{
    E_FLAT = 0,
    E_TEXTURED,
    E_LIGHTED
};

#endif