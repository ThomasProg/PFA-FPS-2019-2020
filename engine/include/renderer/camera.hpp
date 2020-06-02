#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "matrix4x4.hpp"
#include "gTransform.hpp"

#include "transformInterface.hpp"

struct GLFWwindow;

namespace Renderer
{
    struct Camera : public Physics::TransformInterface
    {
    public:
        Core::Maths::Matrix4x4 projection = Core::Maths::Matrix4x4::CreatePerspectiveProjectionMatrix(640, 480, 0.1, 10000, 45.f);

        Core::Maths::Matrix4x4 viewProjectionMatrix;

        void updateViewProjectionMatrix()
        {
            Core::Maths::Matrix4x4::multiply(projection, transform.transformMatrixNode->getWorldMatrix().getInverse(), viewProjectionMatrix);
        }
    };
}

#endif