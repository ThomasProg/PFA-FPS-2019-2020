#ifndef _ENTITY_ID_HPP_
#define _ENTITY_ID_HPP_

#include <functional>

namespace Entity
{
    class EntityID
    {
    private:
        static unsigned int nbEntities;
        
    public:
        unsigned int entityID = 0;

        inline EntityID();
        inline bool operator==(const EntityID&) const;
        inline bool operator<(const EntityID& rhs) const
        {
            return entityID < rhs.entityID;
        }

        EntityID& operator=(const EntityID&) = default;
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