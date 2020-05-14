#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "matrix4x4.hpp"
#include "gTransform.hpp"

struct GLFWwindow;

namespace Renderer
{
    struct Camera
    {
    public:
        // TODO : use GTransform flag to keep inverse transform ?
        Physics::GTransform transform;

        Core::Maths::Matrix4x4 projection = Core::Maths::Matrix4x4::CreatePerspectiveProjectionMatrix(640, 480, 0.1, 10000, 45.f);
    };
}

#endif