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