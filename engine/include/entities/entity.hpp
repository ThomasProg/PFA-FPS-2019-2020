#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

#include <functional>

namespace Entity
{
    class Entity
    {
    private:
        static unsigned int nbEntities;
        
    public:
        unsigned int entityID = 0;

        inline Entity();
        inline bool operator==(const Entity&) const;
        inline bool operator<(const Entity& rhs) const
        {
            return entityID < rhs.entityID;
        }

        Entity& operator=(const Entity&) = default;
    };
}

namespace std
{
    template <>
    struct hash<Entity::Entity>
    {
        inline std::size_t operator()(const Entity::Entity& k) const;
    };
}

#include "entity.inl"

#endif