#include "engine.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

inline bool Core::Engine::isKeyDown(const int key) const
{
    return glfwGetKey(window, key);
}

inline void Core::Engine::endFrame()
{
    float time = glfwGetTime();
    deltaTime = time - lastTime;
    lastTime = time;

    oldHeight = height;
    oldWidth  = width;
    glfwGetWindowSize(window, &width, &height);
    hasWindowSizeChanged = height != oldHeight || width != oldWidth;
    if (hasWindowSizeChanged)
        glViewport(0, 0, width, height);

    // Save mouse location
    lastMouseLocX = currentMouseLocX;
    lastMouseLocY = currentMouseLocY;
    glfwGetCursorPos(window, &currentMouseLocX, &currentMouseLocY);
    deltaMouseLoc = {float((currentMouseLocX - lastMouseLocX) / double(width)), float((currentMouseLocY - lastMouseLocY) / double(height))};

    glfwSwapBuffers(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();
}

inline bool Core::Engine::shouldStop() const
{
    return glfwWindowShouldClose(window);
}

inline void Core::Engine::stop()
{
    glfwSetWindowShouldClose(window, true);
}
