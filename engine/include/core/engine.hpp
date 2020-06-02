#ifndef _ENGINE_HPP_
#define _ENGINE_HPP_

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "vec2.hpp"

#include "demoResourceManager.hpp"
#include "physicsSystem.hpp"
#include "rendererSystem.hpp"
#include "saveSystem.hpp"
#include "audioContext.hpp"

struct GLFWwindow;

namespace Core
{
    class Engine
    {   
    private:
        Save::SaveSystem saveSystem;
        Resources::AudioContext audioContext;

    public:
        DemoResourceManager resourceManager;
        Physics::PhysicsSystem physicsSystem;
        Renderer::RendererSystem rendererSystem;

        /* data */
        GLFWwindow* window;
        float lastTime = 0.f;
        float deltaTime = 0.f;

        double lastMouseLocX = 0.f, lastMouseLocY = 0.f;
        double currentMouseLocX = 0.f, currentMouseLocY = 0.f; 
        Core::Maths::Vec2 deltaMouseLoc = {0.f, 0.f};
        
        int width  = 1000;
        int height = 800;
        int oldWidth  = width;
        int oldHeight = height;    
        bool hasWindowSizeChanged = true;
        ImFont* font;    

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        Engine();
        ~Engine();

        inline void endFrame();

        inline bool shouldStop() const;
        inline void stop();

        inline bool isKeyDown(const int key) const;
        inline bool isMouseButtonDown(const int buttonID) const;
    };
} // namespace Core

#include "engine.inl"

#endif