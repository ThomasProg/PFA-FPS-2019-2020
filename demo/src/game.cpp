#include "game.hpp"

#include "log.hpp"
#include "physicsSystem.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Game::Game()
{
    loadMenu();

    // init 
    init();

    run();
}

void Game::loadMenu()
{
    nextScene = std::make_unique<Menu>(*this);
}
    
void Game::loadLevel(bool isLoaded, bool isEditorMode)
{
    nextScene = std::make_unique<World>(*this, isLoaded, isEditorMode);
}
    
void Game::quitGame()
{
    currentScene = nullptr;
}

void Game::init()
{
    
}

void Game::run()
{
    while (!engine.shouldStop())
    {
        update(); 
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        render();
        renderUI();
        inputs();

        engine.endFrame();

        glClearColor(0.5,0.5,0.9,1.f);
    }
}

void Game::update() 
{
    if (nextScene.get() != nullptr)
    {
        currentScene = std::move(nextScene);
        nextScene = nullptr;
    }

    if (currentScene == nullptr)
    {
        engine.stop();
    }

    Core::Debug::Log::printGLError();
    if (currentScene != nullptr)
        currentScene->update();
}


void Game::renderUI() 
{
    if (currentScene != nullptr)
        currentScene->renderUI();
}


void Game::render() 
{
    if (currentScene != nullptr)
        currentScene->render();
}

void Game::inputs() 
{
    if(currentScene != nullptr)
        currentScene->inputs();

    /*if (engine.isKeyDown(GLFW_KEY_ESCAPE))
    {
        loadMenu();
    }*/
}

