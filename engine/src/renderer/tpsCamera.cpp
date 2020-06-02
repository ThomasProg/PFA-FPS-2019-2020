#include "tpsCamera.hpp"

#include "engine.hpp"

#include "GLFW/glfw3.h"

void Renderer::TPSCamera::setup(Physics::GTransform& parent)
{
    camAnchor.transformMatrixNode = parent.transformMatrixNode->addChild();
    transform.transformMatrixNode = camAnchor.transformMatrixNode->addChild();
    transform.transform.location.z = distanceToTarget;
    transform.UpdateLocalTransformMatrix();
}

void Renderer::TPSCamera::inputs(const Core::Engine& engine)
{
    if (glfwGetKey(engine.window, inputKeys.up))
        camAnchor.transform.rotation.x -= engine.deltaTime * Renderer::TPSCamera::rotationSpeedOnKey;
    if (glfwGetKey(engine.window, inputKeys.left))
        camAnchor.transform.rotation.y -= engine.deltaTime * Renderer::TPSCamera::rotationSpeedOnKey;
    if (glfwGetKey(engine.window, inputKeys.down))
        camAnchor.transform.rotation.x += engine.deltaTime * Renderer::TPSCamera::rotationSpeedOnKey;
    if (glfwGetKey(engine.window, inputKeys.right))
        camAnchor.transform.rotation.y += engine.deltaTime * Renderer::TPSCamera::rotationSpeedOnKey;

    if (isMouseEnabled)
    {
        camAnchor.transform.rotation.x -= engine.deltaMouseLoc.y * mouseSensibility;
        camAnchor.transform.rotation.y -= engine.deltaMouseLoc.x * mouseSensibility;
    }

    camAnchor.UpdateLocalTransformMatrix();
}