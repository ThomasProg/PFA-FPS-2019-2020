#ifndef _DEMO_RESOURCE_MANAGER_HPP_
#define _DEMO_RESOURCE_MANAGER_HPP_

#include "resourceManager.hpp"

// User custom container type :
using ContainerType = unsigned int;

// Forward Declarations :
enum class E_Model   : ContainerType;
enum class E_Texture : ContainerType;
enum class E_Shader  : ContainerType;
enum class E_Audio   : ContainerType;

// User custom typename
using DemoResourceManager = Resources::ResourceManager<E_Model, E_Texture, E_Shader, E_Audio>;

// User custom Enums :
enum class E_Model : ContainerType
{
    E_SPHERE = 0,
    E_BOX,
    E_DOG,
    E_GUN,
    E_TREE,
    E_TREE_NO_LEAVES,
    E_ROCK1,
    E_ROCK2,
    E_GRASS,
};

enum class E_Texture : ContainerType
{
    E_DOG_TEXTURE = 0,
    E_GROUND,
    E_GUN,
};

enum class E_Shader : ContainerType
{
    E_FLAT = 0,
    E_TEXTURED,
    E_LIGHTED
};

enum class E_Audio : ContainerType
{
    E_SHOOT,
};

#endif