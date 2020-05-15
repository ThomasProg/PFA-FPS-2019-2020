#include "entityID.hpp"

inline Entity::EntityID::EntityID()
{
    // entityID = nbEntities;
    // nbEntities++;
}

inline Entity::EntityID::EntityID(unsigned int id) 
    : entityID(id)
{

}

inline bool Entity::EntityID::operator==(const EntityID& rhs) const
{
    return entityID == rhs.entityID;
}

namespace std
{
    inline std::size_t std::hash<Entity::EntityID>::operator()(const Entity::EntityID& k) const
    {
        return hash<typeof(Entity::EntityID::entityID)>()(k.entityID);
    }
}