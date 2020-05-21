#ifndef _FPS_CAMERA_HPP_
#define _FPS_CAMERA_HPP_

#include "camera.hpp"
#include "controllerInterface.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Renderer
{
    struct FPSCamera : public Camera, 
                       public Controller::ControllerInterface
    {
    public:
        static constexpr size_t nbInputKeys = 4;

        union 
        {
            struct 
            {
                unsigned int forward  = GLFW_KEY_W,
                             backward = GLFW_KEY_S,
                             right    = GLFW_KEY_D,
                             left     = GLFW_KEY_A;
            };

            std::array<unsigned int, nbInputKeys> keys;
        } inputKeys;

    public:
        static constexpr float rotationSpeed = 0.1;
        static constexpr float translationSpeed = 40;
        static constexpr float rotationSpeedOnKey = 3.f;
        static constexpr float mouseSensibility = 10.f;

        bool enableInputs = true;

    public:

        void attachTo(Physics::GTransform& parent);

        // Runs camera inputs (camera movement and rotation).
        void inputs(const Core::Engine& engine) override;
    };
}

#endif