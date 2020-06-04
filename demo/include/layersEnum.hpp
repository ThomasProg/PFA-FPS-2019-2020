#ifndef _LAYERS_ENUM_HPP_
#define _LAYERS_ENUM_HPP_

// Instead of enum class, 
// we can convert enums of E_Layers to unsigned int with no cast;
// we deal with unsigned int for layers.
// Used for collisions.
struct LayersEnum
{
    enum E_Layers : unsigned int
    {
        E_DEFAULT = 1 << 0,
        E_ENTITY  = 1 << 1, 
        E_ENEMY   = 1 << 2,
        E_PLAYER  = 1 << 3,
    };
};

#endif