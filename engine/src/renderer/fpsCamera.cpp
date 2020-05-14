#include "fpsCamera.hpp"

#include "engine.hpp"

#include "GLFW/glfw3.h"

#define _IS_MOUSE_ENABLED_ 1

template<class T>
constexpr const T& clamp(const T& v, const T& low, const T& high)
{
    assert(!(high < low) && "Low value is hight than High value");
    return (v < low) ? low : (high < v) ? high : v;
}

void Renderer::FPSCamera::setup(Physics::GTransform& parent)
{
    transform.transformMatrixNode = parent.transformMatrixNode->addChild();
    transform.UpdateLocalTransformMatrix();
    transform.transformMatrixNode->cleanUpdate();
}

void Renderer::FPSCamera::inputs(const Core::Engine& engine)
{
    if (!enableInputs)
        return;

    const Core::Maths::Vec3 forward = transform.transform.getForwardXZVector();
    const Core::Maths::Vec3 right   = transform.transform.getRightXZVector();

    //faster on diagonales : to fix
    if (glfwGetKey(engine.window, GLFW_KEY_UP) || glfwGetKey(engine.window, inputKeys.forward))
    {
        transform.transform.location += forward * (translationSpeed * engine.deltaTime);
    }

    if (glfwGetKey(engine.window, GLFW_KEY_DOWN) || glfwGetKey(engine.window, inputKeys.backward))
    {
        transform.transform.location -= forward * (translationSpeed * engine.deltaTime);
    }

    if (glfwGetKey(engine.window, GLFW_KEY_RIGHT) || glfwGetKey(engine.window, inputKeys.right))
    {
        transform.transform.location += right * (translationSpeed * engine.deltaTime);
    }
    if (glfwGetKey(engine.window, GLFW_KEY_LEFT) || glfwGetKey(engine.window, inputKeys.left))
    {
        transform.transform.location -= right * (translationSpeed * engine.deltaTime);
    }

    #if _IS_MOUSE_ENABLED_
    float deltaMouseX = - engine.deltaMouseLoc.x * mouseSensibility;
    float deltaMouseY = - engine.deltaMouseLoc.y * mouseSensibility; 
    #else
    float deltaMouseX = 0;
    float deltaMouseY = 0;
    #endif

    if (glfwGetKey(engine.window, GLFW_KEY_I))
        deltaMouseY = rotationSpeedOnKey;
    if (glfwGetKey(engine.window, GLFW_KEY_J))
        deltaMouseX = rotationSpeedOnKey;
    if (glfwGetKey(engine.window, GLFW_KEY_K))
        deltaMouseY = -rotationSpeedOnKey;
    if (glfwGetKey(engine.window, GLFW_KEY_L))
        deltaMouseX = -rotationSpeedOnKey;

    transform.transform.rotation.y += deltaMouseX * rotationSpeed;
    transform.transform.rotation.x = clamp(transform.transform.rotation.x + deltaMouseY * rotationSpeed, float(- M_PI / 2.f), float(M_PI / 2.f));

    if (glfwGetKey(engine.window, GLFW_KEY_LEFT_SHIFT))
        transform.transform.location.y -= translationSpeed * engine.deltaTime;
    if (glfwGetKey(engine.window, GLFW_KEY_SPACE))
        transform.transform.location.y += translationSpeed * engine.deltaTime;

    transform.UpdateLocalTransformMatrix();
    transform.transformMatrixNode->setDirtySelfAndChildren();
    transform.transformMatrixNode->cleanUpdate();
}