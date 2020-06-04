#ifndef _TRANSFORM_GRAPH_HPP_
#define _TRANSFORM_GRAPH_HPP_

#include "graph.hpp"

namespace Physics
{
    // Graph was previously templated, since it is a data structure,  
    // so we TransformGraph was a specialized version of Graph.
    // We keep it to not change the user interface, 
    // and in case Graph gets templated again.
    using TransformGraph = Core::DataStructure::Graph;
}

#endif