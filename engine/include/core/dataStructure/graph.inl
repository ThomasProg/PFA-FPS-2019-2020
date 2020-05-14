#include "graph.hpp"

#include <cassert>
#include <algorithm>

#include "saver.hpp"
#include "loader.hpp"

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
constexpr size_t Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::maxChildrenAtLoad; 

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
KEY_TYPE Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::nbGraph {0};

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::Graph()
    : graphID(++nbGraph)
{

}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
void Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::actionThroughChildren(const std::function<void(DATA_TYPE& parent, DATA_TYPE& child)>& func)
{    
    if (parent != nullptr && parent->data != nullptr && this->data != nullptr)
    {
        func(*parent->data, *this->data);
    }

    typename std::unordered_map<KEY_TYPE, std::unique_ptr<CURRENT_GRAPH_TYPE>>::iterator it = children.begin();
    while (it != children.end())
    {
        it->second->action(func);
        it++;
    }
}

// Executes the function for every child and sub childs.
template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
void Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::
        foreach(const std::function<void(CURRENT_GRAPH_TYPE&)>& func)
{
    func(*this);

    typename std::unordered_map<KEY_TYPE, CURRENT_GRAPH_TYPE*>::iterator it = children.begin();
    while (it != children.end())
    {
        it->second->foreach(func);
        it++;
    };
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
typename Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::iterator // return type
    Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::addChild()
{
    // index will be the new graphID
    auto ptr = new CURRENT_GRAPH_TYPE();
    // std::pair<std::unordered_map<KEY_TYPE, CURRENT_GRAPH_TYPE*>::iterator, bool> 
    auto i = children.emplace(nbGraph, ptr);

    if (i.second)
    {
        i.first->second->parent = this;
    }
    else 
    {
        // --nbGraph;
        delete ptr;
    }

    return Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::iterator{i.first->second};
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
typename Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::iterator // return type
    Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::addChild(const KEY_TYPE& key)
{
    // index will be the new graphID
    auto ptr = new CURRENT_GRAPH_TYPE();
    // std::pair<std::unordered_map<KEY_TYPE, CURRENT_GRAPH_TYPE*>::iterator, bool> 
    auto i = children.emplace(key, ptr);

    if (i.second)
    {
        i.first->second->parent = this;
    }
    else 
    {
        // --nbGraph;
        delete ptr;
    }

    if (key > nbGraph)
        nbGraph = key;

    return Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::iterator{i.first->second};
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
void Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::setDirtySelfAndChildren()
{
    isDirty = true;

    typename std::unordered_map<KEY_TYPE, CURRENT_GRAPH_TYPE*>::iterator it = children.begin();
    while (it != children.end())
    {
        it->second->setDirtySelfAndChildren();
        it++;
    };
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>* // return type
    Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>
    ::getHighestDirtyParent()
{
    if (!isDirty)
        return nullptr;

    CURRENT_GRAPH_TYPE* highestDirtyParent = this;

    while (highestDirtyParent != nullptr && highestDirtyParent->isDirty)
    {
        highestDirtyParent = highestDirtyParent->parent;
    }

    return highestDirtyParent;
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
void Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::updateSelfFromParent()
{
    // worldData = updateFunctor(parent->worldData, localData);
    worldData = UPDATE_FUNCTOR::operator()(parent->worldData, localData);
    isDirty = false;
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
void Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::updateSelfAsRoot()
{
    worldData = localData;
    isDirty = false;
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
void Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::updateSelf()
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

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
void Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::updateChildren()
{
    typename std::unordered_map<KEY_TYPE, CURRENT_GRAPH_TYPE*>::iterator it = children.begin();
    while (it != children.end())
    {
        it->second->updateSelfAndChildren();
        it++;
    };
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
void Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::updateSelfAndChildren()
{
    updateSelfFromParent();
    updateChildren();
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
void Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::cleanUpdate()
{
    Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>* dirty = getHighestDirtyParent();
    
    if (dirty)
    {
        dirty->updateChildren();
    }
}

// template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
// void Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::attachTo(
//                                                             Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::iterator& it, 
//                                                             Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, KEY_TYPE>& newParent)
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

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::iterator::iterator(CURRENT_GRAPH_TYPE* graphPtr)
    : graphPtr(graphPtr)
{

}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>& 
    Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::iterator::operator*()
{
    return *graphPtr;
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
const Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>& 
    Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::iterator::operator*() const
{
    return *graphPtr;
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>* // return type
    Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>
        ::iterator::operator->()
{
    return &(*graphPtr);
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
const Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>* // return type
    Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::iterator
        ::operator->() const
{
    return &(*graphPtr);
}

// template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
// bool Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::iterator::operator!=(const iterator& it) const noexcept
// {
//     return listIt != it.listIt || graphPtr != it.graphPtr;
// }

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
bool Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::iterator::isValid() const noexcept
{
    return graphPtr != nullptr;
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
void Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::iterator::setInvalid()
{
    graphPtr = nullptr; 
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
void Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::iterator::erase()
{
    assert(graphPtr != nullptr); // can't erase; the iterator is not valid

    delete this->graphPtr;

    graphPtr = nullptr;
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::~Graph()
{
    for (std::pair<const KEY_TYPE, CURRENT_GRAPH_TYPE*>& child : children)
    {
        child.second->parent = nullptr;
        delete child.second;
    }

    if (parent)
    {
        parent->children.erase(graphID);
    }
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
void Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::
    save(Save::Saver& saver)       
{
    saver.save(this->graphID);
    DATA_SAVER_AND_LOADER::saveLocalData(saver, this->localData);
    DATA_SAVER_AND_LOADER::saveWorldData(saver, this->worldData);
    saver.save(this->children.size());

    typename std::unordered_map<KEY_TYPE, CURRENT_GRAPH_TYPE*>::iterator it = this->children.begin();
    while (it != this->children.end())
    {
        it->second->save(saver);
        it++;
    };
}

template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE>
void Core::DataStructure::Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>::
    loadData(Save::Loader& loader) 
{
    size_t nbChildren;
    loader.load(this->graphID);
    DATA_SAVER_AND_LOADER::loadLocalData(loader, this->localData);
    DATA_SAVER_AND_LOADER::loadWorldData(loader, this->worldData);
    loader.load(nbChildren);

    nbChildren = std::min(nbChildren, maxChildrenAtLoad);

    for (size_t i = 0; i < nbChildren; ++i)
    {
        typename CURRENT_GRAPH_TYPE::iterator it = this->addChild(); 
        it->Graph::loadData(loader);
        it->parent = this;
    }

    loader.tryToDisplayError(__FILE__);
}
