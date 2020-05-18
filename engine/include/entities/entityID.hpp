#ifndef _ENTITY_ID_HPP_
#define _ENTITY_ID_HPP_

#include <functional>

namespace Entity
{
    class EntityID
    {       
    public:
        unsigned int entityID = 0;

        inline EntityID() = default;
        inline EntityID(unsigned int id);
        inline bool operator==(const EntityID&) const;
        inline bool operator<(const EntityID& rhs) const;
        EntityID& operator=(const EntityID&) = default;

        inline void next();
    };
}

namespace std
{
    template <>
    struct hash<Entity::EntityID>
    {
        inline std::size_t operator()(const Entity::EntityID& k) const;
    };
}

#include "entityID.inl"

#endif