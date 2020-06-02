#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_

#include <memory>
#include <functional>
#include <vector>

#include "matrix4x4.hpp"

namespace Core::DataStructure
{
    // By inheriting the functor, we can use the desired function.
    // If we stored it as a member, even if the functor would have no members (variables),
    // the size would have grown up.
    class Graph
    {  
    protected:
        // No smart ptr, too much nodes would weight too much,
        // allocation is managed
        std::vector<std::unique_ptr<Graph>> children;
        Graph* parent = nullptr;

        bool isDirty = false;

    public:
        Core::Maths::Matrix4x4 localData = Core::Maths::Matrix4x4::identity(4);;
    protected:
        Core::Maths::Matrix4x4 worldData = Core::Maths::Matrix4x4::identity(4);;

    public:
        inline const Core::Maths::Matrix4x4& getWorldMatrix() const
        {
            return worldData;
        } 

        inline Graph() = default;
        inline Graph(const Graph& rhs) = delete;
        inline ~Graph() = default;

        inline Graph& operator=(const Graph& rhs) = delete;

        // // Executes the function for every child and sub childs.
        // void foreach(const std::function<void(Graph&)>& func);

        // Set the dirty flag to true for this object and its children.
        inline void setDirtySelfAndChildren();
        
        // Considers the parents are not dirty, and update this object current worldData.
        // Considers this node has a parent (so parent != nullptr), 
        // and uses the parent world transform matrix to compute its own world transform matrix.
        inline void updateSelfFromParent();
        // Sets the world matrix the same as the local matrix.
        inline void updateSelfAsRoot();
        // Switches on updateSelfFromParent() or updateSelfAsRoot()
        // if the node is a root or not.
        // If you already know, 
        // consider using updateSelfFromParent() or updateSelfAsRoot() instead.
        inline void updateSelf();
        // Considers the parents as not dirty, and update the children worldData.
        inline void updateChildren();
        // Considers the parents as not dirty, and update this object and its children worldData.
        inline void updateSelfAndChildren();

        // Returns the highest dirty parent in the hierarchy.
        inline Graph* getHighestDirtyParent();

        // Get the highest dirty parent, and update every children from there.
        inline void cleanUpdate();

        // "Iterators" are returned to the user.
        // The user can then use them to erase the object, move it into the graph, etc 
        // Project's Convention is dropped to adopt the standard name, and so be more intuitive.
        class iterator
        {
        private:
            // pointer towards pointed graph
            Graph* graphPtr = nullptr;
            
        public:
            inline iterator() = default;
            inline iterator(const iterator&) = default;
            inline iterator(Graph* graphPtr);
            inline ~iterator() = default;

            inline Graph& operator*();
            inline const Graph& operator*() const;
            inline Graph* operator->();
            inline const Graph* operator->() const;

            // bool operator!=(const iterator& it) const noexcept;

            inline bool isValid() const noexcept;
            inline void setInvalid();
            inline void erase();

            // We can't use attachTo() without accessing the iterator's members
            // and without breaking encapsulation.
            friend class Graph;
        };

        // Add child to the graph, setting the parent to this, 
        // and returns an iterator to it. 
        inline iterator addChild(); 


        // // // Change the parent of the node pointed by "it" to "newParent".
        // // // Inputs :
        // // // iterator it : the iterator used for the current node we want to change parent
        // // // Graph& newParent : the graph which will become the parent of it
        // // static void attachTo(iterator& it, Graph& newParent);
    };
}

#include "graph.inl"

#endif