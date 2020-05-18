#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_

#include <functional>
#include <unordered_map>
#include <memory>

#include "saveInterface.hpp"

namespace Core::DataStructure
{
    //
    // DATA_TYPE : The type of the data we want to store (example : Matrix4x4)
    //
    // UPDATE_FUNCTOR : A functor (a class overloading the () operator), this will be used to update the data
    //
    // DATA_SAVER_AND_LOADER : A class containing the following functions : 
    //      - static void saveLocalData(Save::Saver&  saver,  const DATA_TYPE& data);
    //      - static void saveWorldData(Save::Saver&  saver,  const DATA_TYPE& data);
    //      - static void loadLocalData(Save::Loader& loader, DATA_TYPE& data);
    //      - static void loadWorldData(Save::Loader& loader, DATA_TYPE& data);
    // Inheriting from SaveInterface is costly, because it would make the function virtual;
    // Instead, by using these functions, we have no memory overhead
    // 
    // KEY_TYPE : The type of the key to retrieve the wanted DATA_TYPE instance. Default is unsigned int. 
    //
    template<typename DATA_TYPE, typename UPDATE_FUNCTOR, typename DATA_SAVER_AND_LOADER, typename KEY_TYPE = unsigned int>
    // By inheriting the functor, we can use the desired function.
    // If we stored it as a member, even if the functor would have no members (variables),
    // the size would have grown up.
    class Graph : UPDATE_FUNCTOR, public Save::SaveInterface
    {  
    public:
        // class iterator;

        using CURRENT_GRAPH_TYPE = Graph<DATA_TYPE, UPDATE_FUNCTOR, DATA_SAVER_AND_LOADER, KEY_TYPE>;

    protected:
        static constexpr size_t maxChildrenAtLoad = 100;
        static KEY_TYPE nbGraph;

        // No smart ptr, too much nodes would weight too much,
        // allocation is managed
        std::unordered_map<KEY_TYPE, CURRENT_GRAPH_TYPE*> children;
        CURRENT_GRAPH_TYPE* parent = nullptr;

        bool isDirty = false;

    public:
        KEY_TYPE graphID;

        DATA_TYPE localData;
        DATA_TYPE worldData;

        Graph();
        Graph(const CURRENT_GRAPH_TYPE& rhs) = delete;
        virtual ~Graph();

        CURRENT_GRAPH_TYPE& operator=(const CURRENT_GRAPH_TYPE& rhs) = delete;

        // Executes the function for every child and sub childs.
        void actionThroughChildren(const std::function<void(DATA_TYPE& parent, DATA_TYPE& child)>& func);

        // Executes the function for every child and sub childs.
        void foreach(const std::function<void(CURRENT_GRAPH_TYPE&)>& func);

        // Set the dirty flag to true for this object and its children.
        void setDirtySelfAndChildren();
        
        // Considers the parents are not dirty, and update this object current worldData.
        // Considers this node has a parent (so parent != nullptr), 
        // and uses the parent world transform matrix to compute its own world transform matrix.
        void updateSelfFromParent();
        // Sets the world matrix the same as the local matrix.
        void updateSelfAsRoot();
        // Switches on updateSelfFromParent() or updateSelfAsRoot()
        // if the node is a root or not.
        // If you already know, 
        // consider using updateSelfFromParent() or updateSelfAsRoot() instead.
        void updateSelf();
        // Considers the parents as not dirty, and update the children worldData.
        void updateChildren();
        // Considers the parents as not dirty, and update this object and its children worldData.
        void updateSelfAndChildren();

        // Returns the highest dirty parent in the hierarchy.
        CURRENT_GRAPH_TYPE* getHighestDirtyParent();

        // Get the highest dirty parent, and update every children from there.
        void cleanUpdate();

        // "Iterators" are returned to the user.
        // The user can then use them to erase the object, move it into the graph, etc 
        // Project's Convention is dropped to adopt the standard name, and so be more intuitive.
        class iterator
        {
        private:
            using GraphListIt = typename std::unordered_map<KEY_TYPE, CURRENT_GRAPH_TYPE>::iterator;

            // pointer towards pointed graph
            CURRENT_GRAPH_TYPE* graphPtr = nullptr;
            
        public:
            iterator() = default;
            iterator(const iterator&) = default;
            iterator(CURRENT_GRAPH_TYPE* graphPtr);
            ~iterator() = default;

            CURRENT_GRAPH_TYPE& operator*();
            const CURRENT_GRAPH_TYPE& operator*() const;
            CURRENT_GRAPH_TYPE* operator->();
            const CURRENT_GRAPH_TYPE* operator->() const;

            // bool operator!=(const iterator& it) const noexcept;

            bool isValid() const noexcept;
            void setInvalid();
            void erase();

            // We can't use attachTo() without accessing the iterator's members
            // and without breaking encapsulation.
            friend class Graph<DATA_TYPE, UPDATE_FUNCTOR, KEY_TYPE>;
        };

        // Add child to the graph, setting the parent to this, 
        // and returns an iterator to it. 
        iterator addChild(); 
        iterator addChild(const KEY_TYPE& key); 


        // // Change the parent of the node pointed by "it" to "newParent".
        // // Inputs :
        // // iterator it : the iterator used for the current node we want to change parent
        // // CURRENT_GRAPH_TYPE& newParent : the graph which will become the parent of it
        // static void attachTo(iterator& it, CURRENT_GRAPH_TYPE& newParent);

        void save(Save::Saver& saver)       override;
        void loadData(Save::Loader& loader) override;
    };
}

#include "graph.inl"

#endif