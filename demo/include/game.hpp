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

    static constexpr const char* savedFilename = "bin/world.dat";
    bool isAzerty {false};
    bool isHighQuality = true;
    
public:
    Game();

    void loadResources();

    void loadMenu();
    void loadLevel(bool isLoaded, bool isEditorMode = false);
    void quitGame();

    void run();
    void inputs();

    void update() override;
    void renderUI() override;
    void render() override;
};

#endif 