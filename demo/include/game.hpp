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
    std::unique_ptr<Scene> currentScene;
    // std::unique_ptr<World> world;
    // std::unique_ptr<Menu> menu;

    std::unique_ptr<Scene> nextScene;

    static constexpr const char* savedFilename = "bin/world.dat";
    bool isAzerty {false};
    
public:
    Game();

    void loadMenu();
    void loadLevel(bool isLoaded, bool isEditorMode = false);
    void quitGame();

    void run();
    void inputs();
    //int the world
    void init();

    void update() override;
    void renderUI() override;
    void render() override;
};

#endif 