#ifndef _TRANSFORM_INTERFACE_HPP_
#define _TRANSFORM_INTERFACE_HPP_

#include "gTransform.hpp"

namespace Physics
{
    class TransformInterface
    {
    public:
        Physics::GTransform transform;
    
    public:
    
        void setTransform(const Physics::Transform& newTransform)
        {
            // Be sure to set the parent first.
            assert(transform.transformMatrixNode.isValid());

            this->transform.transform = newTransform;
            transform.UpdateLocalTransformMatrix();
            transform.transformMatrixNode->setDirtySelfAndChildren();
        }
        void setTransformParent(Physics::TransformGraph& transformParent)
        {
            transform.transformMatrixNode = transformParent.addChild();
            assert(transform.transformMatrixNode.isValid());
        }
    };
}

#endif