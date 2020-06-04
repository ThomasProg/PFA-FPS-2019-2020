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

        Core::Maths::Matrix4x4 localData = Core::Maths::Matrix4x4::identity(4);
    
        // Normally, we would update it each time we use "setLocalMatrix()".
        // However, in this case, we would also update children's "worldData" at the same time.
        // If we use multiple times setLocalMatrix() in a graph then in its children,
        // it would mean we'd have to compute the world matrix again each time for each children.
        // To prevent that, we cache the worldData, and update it only when it is necessary.
        // However, the user could want to use "worldData" in a const function ;
        // after all, he doesn't want to modify "worldData", he just wants to get it!
        // That is why we put the "mutable" keyword to both "worldData" and "isDirty".
        mutable Core::Maths::Matrix4x4 worldData;

    private:
        mutable bool isDirty = true;

        // Set self and children dirty flag to true.
        inline void setDirty() noexcept;

    public:
        inline Graph() = default;
        inline Graph(const Graph& rhs) = delete;
        inline ~Graph() = default;

        inline Graph& operator=(const Graph& rhs) = delete;

        inline const Core::Maths::Matrix4x4& getLocalMatrix() const noexcept;
        inline void setLocalMatrix(Core::Maths::Matrix4x4&& mat);
        inline void setLocalMatrix(const Core::Maths::Matrix4x4& mat);

        inline const Core::Maths::Matrix4x4& getWorldMatrix() const;

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

            inline iterator& operator=(const iterator&) = default;

            inline Graph& operator*();
            inline const Graph& operator*() const;
            inline Graph* operator->();
            inline const Graph* operator->() const;

            inline bool operator!=(const iterator& it) const noexcept;

            inline bool isValid() const noexcept;
            inline void setInvalid() noexcept;
            inline void erase(); // O(n)

            // We can't use attachTo() without accessing the iterator's members
            // without using a friend,
            // or it would break encapsulation.
            friend class Graph;
        };

        // Add child to the graph, setting the parent to this, 
        // and returns an iterator to it. 
        inline iterator addChild(); 


        // Change the parent of the node pointed by "it" to "newParent".
        // Inputs :
        // iterator it : the iterator used for the current node we want to change parent
        // Graph& newParent : the graph which will become the parent of it
        static inline void attachTo(iterator& it, Graph& newParent); // O(n)
    };
}

#include "graph.inl"

#endif