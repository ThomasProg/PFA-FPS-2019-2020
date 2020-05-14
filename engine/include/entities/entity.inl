#include "entity.hpp"

inline Entity::Entity::Entity()
{
    entityID = nbEntities;
    nbEntities++;
}

inline bool Entity::Entity::operator==(const Entity& rhs) const
{
    return entityID == rhs.entityID;
}

namespace std
{
    inline std::size_t std::hash<Entity::Entity>::operator()(const Entity::Entity& k) const
    {
        return hash<typeof(Entity::Entity::entityID)>()(k.entityID);
    }
}