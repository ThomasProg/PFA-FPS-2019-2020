#ifndef _TRANSFORM_GRAPH_HPP_
#define _TRANSFORM_GRAPH_HPP_

#include "graph.hpp"
#include "matrix4x4.hpp"
#include "transform.hpp"

namespace Physics
{
    class TransformGraphFunctor
    {
    public:
        Core::Maths::Matrix4x4 operator()(const Core::Maths::Matrix4x4& world, const Core::Maths::Matrix4x4& local)
        {
            return world * local;
        }
    };

    struct GraphKey 
    {
        unsigned int entityIndex;

        GraphKey() : entityIndex(0) {}
        GraphKey(unsigned int index) : entityIndex(index) {}

        bool operator==(const GraphKey& rhs) const
        {
            return entityIndex == rhs.entityIndex;
        }     

        bool operator<(const GraphKey& rhs) const
        {
            return entityIndex < rhs.entityIndex;
        }  

        GraphKey operator+(const unsigned int rhs)
        {
            return GraphKey{entityIndex + rhs};
        }     

        GraphKey& operator++()
        {
            ++entityIndex;
            return *this;
        }           
    };
    
    // class DataSaverAndLoader
    // {
    // public:
    //     static void saveLocalData(Save::Saver&  saver,  const Core::Maths::Matrix4x4& data)
    //     {
    //         saver.savePtr(data.matrix, data.nbColumns * data.nbLines);
    //     }

    //     static void saveWorldData(Save::Saver&  saver,  const Core::Maths::Matrix4x4& data)
    //     {
    //         saver.savePtr(data.matrix, data.nbColumns * data.nbLines);
    //     }

    //     static void loadLocalData(Save::Loader& loader, Core::Maths::Matrix4x4& data)
    //     {
    //         loader.loadPtr(data.matrix);
    //     }

    //     static void loadWorldData(Save::Loader& loader, Core::Maths::Matrix4x4& data)
    //     {
    //         loader.loadPtr(data.matrix);
    //     }

    // };

    using TransformGraph = Core::DataStructure::Graph;//<Core::Maths::Matrix4x4, TransformGraphFunctor, DataSaverAndLoader, GraphKey>;
}

namespace std
{
    template <>
    struct hash<Physics::GraphKey>
    {
        inline std::size_t operator()(const Physics::GraphKey& k) const
        {
            return k.entityIndex;
        }
    };
}

#endif