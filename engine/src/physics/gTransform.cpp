#include "gTransform.hpp"

void Physics::GTransform::UpdateLocalTransformMatrix()
{
    if (!transformMatrixNode.isValid())
        return;

    transformMatrixNode->setLocalMatrix(Core::Maths::Matrix4x4::CreateTRSMatrix(transform.scale, 
                                                                                transform.location, 
                                                                                transform.rotation));
}