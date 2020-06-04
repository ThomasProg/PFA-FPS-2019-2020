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
        }
        void setTransformParent(Physics::TransformGraph& transformParent)
        {
            if (transform.transformMatrixNode.isValid())
                transform.transformMatrixNode.erase();
            transform.transformMatrixNode = transformParent.addChild();
            assert(transform.transformMatrixNode.isValid());
        }
        inline void setTransformParent(Physics::GTransform& transformParent)
        {
            assert(transformParent.transformMatrixNode.isValid());
            setTransformParent(*transformParent.transformMatrixNode);
        }
        inline void setTransformParent(Physics::TransformInterface& transformParent)
        {
            setTransformParent(transformParent.transform);
        }
    };
}

#endif