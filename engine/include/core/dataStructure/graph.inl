#include "graph.hpp"

#include <cassert>
#include <algorithm>

// Set self and children dirty flag to true.
inline void Core::DataStructure::Graph::setDirty() noexcept
{
    // If the graph is already dirty, so are its children,
    // and we do not have to continue.
    if (isDirty)
        return;

    isDirty = true;
    for (const std::unique_ptr<Graph>& child : children)
    {
        child->setDirty();
    }
}

inline typename Core::DataStructure::Graph::iterator // return type
    Core::DataStructure::Graph::addChild()
{
    children.emplace_back(std::make_unique<Graph>());

    Graph* newGraph = children.back().get();
    newGraph->parent = this;
    return Core::DataStructure::Graph::iterator{newGraph};
}

inline void Core::DataStructure::Graph::attachTo(iterator& it, Graph& newParent)
{
    assert(it.isValid()); // can't erase; the iterator is not valid

    if (it.graphPtr->parent == nullptr)
        return;

    std::vector<std::unique_ptr<Graph>>& childrenRef = it.graphPtr->parent->children;

    std::vector<std::unique_ptr<Graph>>::iterator childrenIt = childrenRef.begin();
    while (childrenIt != childrenRef.end())
    {
        if ((*childrenIt).get() == it.graphPtr)
        {
            newParent.children.emplace_back(childrenIt->release());
            childrenRef.erase(childrenIt);
            break;
        }
    }
}

inline const Core::Maths::Matrix4x4& Core::DataStructure::Graph::getLocalMatrix() const noexcept
{
    return localData;
}

inline void Core::DataStructure::Graph::setLocalMatrix(Core::Maths::Matrix4x4&& mat)
{
    localData = std::move(mat);
    setDirty();
}

inline void Core::DataStructure::Graph::setLocalMatrix(const Core::Maths::Matrix4x4& mat)
{
    localData = mat;
    setDirty();
}

inline const Core::Maths::Matrix4x4& Core::DataStructure::Graph::getWorldMatrix() const
{
    if (isDirty)
    {
        if (parent != nullptr)
            Core::Maths::Matrix4x4::multiply(parent->getWorldMatrix(), localData, worldData);
        else 
            worldData = localData;

        isDirty = false;
    }
    return worldData;
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


inline bool Core::DataStructure::Graph::iterator::operator!=(const iterator& it) const noexcept
{
    return graphPtr != it.graphPtr;
}

inline bool Core::DataStructure::Graph::iterator::isValid() const noexcept
{
    return graphPtr != nullptr;
}

inline void Core::DataStructure::Graph::iterator::setInvalid() noexcept
{
    graphPtr = nullptr; 
}

inline void Core::DataStructure::Graph::iterator::erase()
{
    assert(isValid()); // can't erase; the iterator is not valid

    if (graphPtr->parent == nullptr)
        return;

    std::vector<std::unique_ptr<Graph>>& childrenRef = graphPtr->parent->children;

    std::vector<std::unique_ptr<Graph>>::iterator it = childrenRef.begin();
    while (it != childrenRef.end())
    {
        if ((*it).get() == this->graphPtr)
        {
            childrenRef.erase(it);
            break;
        }
    }

    setInvalid();
}
