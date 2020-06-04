#include "camera.hpp"

inline void Renderer::Camera::updateViewProjectionMatrix()
{
    Core::Maths::Matrix4x4::multiply(projection, transform.transformMatrixNode->getWorldMatrix().getInverse(), viewProjectionMatrix);
}