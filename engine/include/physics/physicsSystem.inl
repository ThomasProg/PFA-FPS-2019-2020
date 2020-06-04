#include "physicsSystem.hpp"

template<typename T>
Physics::PhysicsSystem::iterator<T>::iterator(unsigned int index)
    : arrayIndex(index)
{

}