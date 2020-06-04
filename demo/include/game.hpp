#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "scene.hpp"
#include "engine.hpp"
#include "world.hpp"
#include "menu.hpp"

#include <memory>

class Game : public Resources::Scene
{
public:
    Core::Engine engine;
    // Pointer towards the current loaded scene.
    std::unique_ptr<Scene> currentScene;
    // Pointer towards the next scene that will be loaded.
    // If no scene is planned to be loaded, 
    // the "nextScene" is nullptr.
    std::unique_ptr<Scene> nextScene;

    bool isHighQuality = true;
    
public:
    // Loads Resources, 
    // loads Menu,
    // and runs the game loop.
    Game();

    // Loads textures, models, so   unds and shaders.
    void loadResources();

    void loadMenu();
    void loadLevel();
    void quitGame();

    // Runs the game loop.
    void run();
    void inputs();

    // Called every frame
    void update() override;
    void renderUI() override;
    void render() override;
};

#endif 