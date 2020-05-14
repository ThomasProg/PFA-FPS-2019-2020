#include "gTransform.hpp"

void Physics::GTransform::UpdateLocalTransformMatrix()
{
    // assert(transformMatrixNode.isValid());
    if (!transformMatrixNode.isValid())
        return;

    transformMatrixNode->localData = Core::Maths::Matrix4x4::CreateTRSMatrix(transform.scale, 
                                                                             transform.location, 
                                                                             transform.rotation);

    transformMatrixNode->setDirtySelfAndChildren();
}