#ifndef _GTRANSFORM_HPP_
#define _GTRANSFORM_HPP_

#include "transform.hpp"
#include "transformGraph.hpp"

namespace Physics
{
    // Transform containing a matrix in the graph
    struct GTransform
    {
        // local transform
        Transform transform;

        TransformGraph::iterator transformMatrixNode;

        void UpdateLocalTransformMatrix();
    };
}

#endif