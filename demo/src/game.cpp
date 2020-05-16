#include "game.hpp"

#include "log.hpp"
#include "physicsSystem.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "primitives.hpp"

Game::Game()
{
    loadResources();
    loadMenu();

    run();
}

void Game::loadResources()
{
    // ===== Set up resources ===== //
    {
        Resources::Model m(Primitives::CreateSphere(20, 20));
        m.setupModel();
        engine.resourceManager.add(std::move(m), E_Model::E_SPHERE);
    }
    {
        engine.resourceManager.add(Resources::Texture{"resources/textures/ground.jpg"}, E_Texture::E_GROUND);
    }
    {
        Resources::Model model;
        model.loadOBJ("resources/obj/dog.obj");
        for (Core::Maths::Vec3& pos : model.positions)
        {
            pos /= 10.f;
            float temp = pos.z;
            pos.z = pos.y;
            pos.y = temp;

            pos.y -= 1.1f;
            pos.z /= 2.f;
            pos.y /= 1.3f;

            pos.x *= -1;
        }
        model.setupModel();
        engine.resourceManager.add(std::move(model), E_Model::E_DOG);
        engine.resourceManager.add(Resources::Texture{"resources/obj/Dog_diffuse.jpg"}, E_Texture::E_DOG_TEXTURE);
    }
    {
        Resources::Model m(Primitives::CreateCube());
        m.setupModel();
        engine.resourceManager.add(std::move(m), E_Model::E_BOX);
    }
    engine.resourceManager.add(Resources::Shader{"resources/shaders/flatColor.vert", "resources/shaders/flatColor.frag"}, E_Shader::E_FLAT);
    engine.resourceManager.add(Resources::Shader{"resources/shaders/vs.vert", "resources/shaders/fsWithoutLight.frag"}, E_Shader::E_TEXTURED);
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
        currentScene->load();
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

