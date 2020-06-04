#include "game.hpp"

#include "log.hpp"
#include "physicsSystem.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "primitives.hpp"

#include "phongFlat.hpp"
#include "flatShader.hpp"

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
        engine.resourceManager.add(Resources::Model{"resources/obj/singleTree.obj"}, E_Model::E_TREE);
        engine.resourceManager.add(Resources::Model{"resources/obj/treeWithoutLeaves.obj"}, E_Model::E_TREE_NO_LEAVES);
        engine.resourceManager.add(Resources::Model{"resources/obj/rock1.obj"}, E_Model::E_ROCK1);
        engine.resourceManager.add(Resources::Model{"resources/obj/rock2.obj"}, E_Model::E_ROCK2);
        engine.resourceManager.add(Resources::Model{"resources/obj/grass.obj"}, E_Model::E_GRASS);
    }
    {
        Resources::Model model;
        model.loadOBJ("resources/obj/dog.obj");
        for (Core::Maths::Vec3& pos : model.positions)
        {
            pos /= 30.f;
            std::swap(pos.y, pos.z);

            pos.z *= -1;
            pos.y -= 1;
        }

        for (Core::Maths::Vec3& normal : model.normals)
        {
            std::swap(normal.y, normal.z);

            normal.z *= -1;
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
    {
        Resources::Model model;
        model.loadOBJ("resources/obj/L-M14.obj");
        model.setupModel();
        engine.resourceManager.add(std::move(model), E_Model::E_GUN);
        engine.resourceManager.add(Resources::Texture{"resources/obj/M14warna.png"}, E_Texture::E_GUN);
    }
    {
        Resources::Model model;
        model.loadOBJ("resources/obj/cat.obj");
         for (Core::Maths::Vec3& pos : model.positions)
        {
            pos /= 30.f;
            std::swap(pos.y, pos.z);
            std::swap(pos.x, pos.z);

            pos.y -= 1.02f;
        }
        for (Core::Maths::Vec3& normal : model.normals)
        {
            std::swap(normal.y, normal.z);
            std::swap(normal.x, normal.z);
        }
        model.setupModel();
        engine.resourceManager.add(std::move(model), E_Model::E_CAT);
        engine.resourceManager.add(Resources::Texture{"resources/obj/cat.jpg"}, E_Texture::E_CAT);
    }

    {
        engine.resourceManager.add(std::make_unique<Resources::FlatShader>
            ("resources/shaders/flatColor.vert", "resources/shaders/flatColor.frag"), E_Shader::E_FLAT);
            
        engine.resourceManager.add(std::make_unique<Resources::PhongFlat> 
            ("resources/shaders/vs.vert", "resources/shaders/lightsFlatColor.frag"), E_Shader::E_LIGHTED_FLATCOLOR);

        // works with flatShader; texture is bound as long as it is not nullptr
        engine.resourceManager.add(std::make_unique<Resources::FlatShader>
            ("resources/shaders/vs.vert", "resources/shaders/fsWithoutLight.frag"), E_Shader::E_TEXTURED); 

        engine.resourceManager.add(std::make_unique<Resources::PhongFlat> 
            ("resources/shaders/vs.vert", "resources/shaders/dynamicLightsEffects.frag"), E_Shader::E_LIGHTED);
    }

    {
        engine.resourceManager.add(Resources::Audio{"resources/audio/pew.wav"}, E_Audio::E_SHOOT);
        engine.resourceManager.add(Resources::Audio{"resources/audio/waf.wav"}, E_Audio::E_DOG_ATTACK);
        engine.resourceManager.add(Resources::Audio{"resources/audio/miaw.wav"}, E_Audio::E_BOSS_ATTACK);
    }
}

void Game::loadMenu()
{
    nextScene = std::make_unique<Menu>(*this);
}
    
void Game::loadLevel()
{
    nextScene = std::make_unique<World>(*this);
}
    
void Game::quitGame()
{
    currentScene = nullptr;
}

void Game::run()
{        
    glClearColor(0.1,0.1,0.2,1.f); // dark blue

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
}

