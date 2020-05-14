#ifndef _TPS_CAMERA_HPP_
#define _TPS_CAMERA_HPP_

#include "camera.hpp"
#include "controllerInterface.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Renderer
{
    struct TPSCamera : public Camera, public Controller::ControllerInterface
    {
    public:
        static constexpr float distanceToTarget = 20.f;
        static constexpr float rotationSpeedOnKey = 3.f;
        
        static constexpr float mouseSensibility = 0.5f; // for inputs
        
        Physics::GTransform camAnchor;

        // ====== Input Keys ====== //
    public:
        static constexpr size_t nbInputKeys = 4;

        union 
        {
            struct 
            {
                unsigned int up    = GLFW_KEY_I,
                             down  = GLFW_KEY_K,
                             right = GLFW_KEY_L,
                             left  = GLFW_KEY_J;
            };

            std::array<unsigned int, nbInputKeys> keys;
        } inputKeys;

        bool isMouseEnabled = true;

    public:
        using InputsKeys = std::array<unsigned int, nbInputKeys>;
        // ======================== //

        void setup(Physics::GTransform& parent);

        // Runs camera inputs (camera movement and rotation).
        void inputs(const Core::Engine& engine) override;
    };
}

#endif