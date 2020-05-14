#include "engine.hpp"

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

Core::Engine::Engine()
{
    if (!glfwInit())
    {
        std::cout << "Fail to init GLFW" << std::endl;
        return;
    }

    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(width, height, "Platformer", NULL, NULL);

    if (!window)
    {
        // Window or OpenGL context creation failed
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Save mouse location
    glfwGetCursorPos(window, &currentMouseLocX, &currentMouseLocY);
    lastMouseLocX = currentMouseLocX;
    lastMouseLocY = currentMouseLocY;
    deltaMouseLoc = {(currentMouseLocX - lastMouseLocX) / width, (currentMouseLocY - lastMouseLocY) / height};

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);  
    glEnable(GL_DEPTH_TEST);
}

Core::Engine::~Engine()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    // We have to destruct models before glfwTeminate(),
    // or we won't be able to use glDeleteVertexArrays and other free functions.
    resourceManager.clear();

    glfwDestroyWindow(window);
    glfwTerminate();
}
