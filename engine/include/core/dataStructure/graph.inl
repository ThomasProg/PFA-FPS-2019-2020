#include "graph.hpp"

#include <cassert>
#include <algorithm>

inline typename Core::DataStructure::Graph::iterator // return type
    Core::DataStructure::Graph::addChild()
{
    // std::pair<std::unordered_map<Core::Maths::Matrix4x4, Graph*>::iterator, bool> 
    children.emplace_back(std::make_unique<Graph>());

    Graph* newGraph = children.back().get();
    newGraph->parent = this;
    return Core::DataStructure::Graph::iterator{newGraph};
}

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
