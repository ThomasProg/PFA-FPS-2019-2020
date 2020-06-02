#include "graph.hpp"

#include <cassert>
#include <algorithm>

// // Executes the function for every child and sub childs.
// void Core::DataStructure::Graph::
//         foreach(const std::function<void(Graph&)>& func)
// {
//     func(*this);

//     typename std::vector<std::unique_ptr<Graph>>::iterator it = children.begin();
//     while (it != children.end())
//     {
//         (*it)->foreach(func);
//         it++;
//     };
// }

inline typename Core::DataStructure::Graph::iterator // return type
    Core::DataStructure::Graph::addChild()
{
    // std::pair<std::unordered_map<Core::Maths::Matrix4x4, Graph*>::iterator, bool> 
    children.emplace_back(std::make_unique<Graph>());

    Graph* newGraph = children.back().get();
    newGraph->parent = this;
    return Core::DataStructure::Graph::iterator{newGraph};
}

inline void Core::DataStructure::Graph::setDirtySelfAndChildren()
{
    isDirty = true;

    typename std::vector<std::unique_ptr<Graph>>::iterator it = children.begin();
    while (it != children.end())
    {
        (*it)->setDirtySelfAndChildren();
        it++;
    };
}

inline Core::DataStructure::Graph* // return type
    Core::DataStructure::Graph
    ::getHighestDirtyParent()
{
    if (!isDirty)
        return nullptr;

    Graph* highestDirtyParent = this;

    while (highestDirtyParent != nullptr && highestDirtyParent->isDirty)
    {
        highestDirtyParent = highestDirtyParent->parent;
    }

    return highestDirtyParent;
}

inline void Core::DataStructure::Graph::updateSelfFromParent()
{
    // worldData = updateFunctor(parent->worldData, localData);
    worldData = parent->worldData * localData;
    isDirty = false;
}

inline void Core::DataStructure::Graph::updateSelfAsRoot()
{
    worldData = localData;
    isDirty = false;
}

inline void Core::DataStructure::Graph::updateSelf()
{
    if (parent != nullptr)
    {
        updateSelfFromParent();
    }
    else 
    {
        updateSelfAsRoot();
    }
}

inline void Core::DataStructure::Graph::updateChildren()
{
    typename std::vector<std::unique_ptr<Graph>>::iterator it = children.begin();
    while (it != children.end())
    {
        (*it)->updateSelfAndChildren();
        it++;
    };
}

inline void Core::DataStructure::Graph::updateSelfAndChildren()
{
    updateSelfFromParent();
    updateChildren();
}

inline void Core::DataStructure::Graph::cleanUpdate()
{
    Core::DataStructure::Graph* dirty = getHighestDirtyParent();
    
    if (dirty)
    {
        dirty->updateChildren();
    }
}

//
// inline void Core::DataStructure::Graph::attachTo(
//                                                             Core::DataStructure::Graph::iterator& it, 
//                                                             Core::DataStructure::Graph<Core::Maths::Matrix4x4, UPDATE_FUNCTOR, Core::Maths::Matrix4x4>& newParent)
// {
//     if (it.graphPtr != nullptr) // if node existing, we don't want to reallocate the node
//         newParent.children.splice(it.listIt, it.graphPtr->children);
//     else // if node not existing, create a new node
//         it = newParent.addChild(); 

//     it.graphPtr->parent = &newParent;
// }

//////////////////////////////////////////////////
// ================= ITERATOR ================= //
//////////////////////////////////////////////////

inline Core::DataStructure::Graph::iterator::iterator(Graph* graphPtr)
    : graphPtr(graphPtr)
{

}

inline Core::DataStructure::Graph& 
    Core::DataStructure::Graph::iterator::operator*()
{
    return *graphPtr;
}

inline const Core::DataStructure::Graph& 
    Core::DataStructure::Graph::iterator::operator*() const
{
    return *graphPtr;
}

inline Core::DataStructure::Graph* // return type
    Core::DataStructure::Graph
        ::iterator::operator->()
{
    assert(graphPtr != nullptr);
    return &(*graphPtr);
}

inline const Core::DataStructure::Graph* // return type
    Core::DataStructure::Graph::iterator
        ::operator->() const
{
    assert(graphPtr != nullptr);
    return &(*graphPtr);
}

//
// bool Core::DataStructure::Graph::iterator::operator!=(const iterator& it) const noexcept
// {
//     return listIt != it.listIt || graphPtr != it.graphPtr;
// }

inline bool Core::DataStructure::Graph::iterator::isValid() const noexcept
{
    return graphPtr != nullptr;
}

inline void Core::DataStructure::Graph::iterator::setInvalid()
{
    graphPtr = nullptr; 
}

inline void Core::DataStructure::Graph::iterator::erase()
{
    assert(graphPtr != nullptr); // can't erase; the iterator is not valid

    std::vector<std::unique_ptr<Graph>>::iterator it = graphPtr->parent->children.begin();
    while (it != graphPtr->parent->children.end())
    {
        if ((*it).get() == this->graphPtr)
        {
            graphPtr->parent->children.erase(it);
            break;
        }
    }

    // iterator becomes invalid
    // graphPtr = nullptr;
}
