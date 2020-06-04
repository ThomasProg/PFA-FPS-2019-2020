#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "scene.hpp"

#include "transformGraph.hpp"
#include "physicsSystem.hpp"
#include "saveSystem.hpp"

#include "mesh.hpp"

#include "fpsCamera.hpp"
#include "tpsCamera.hpp"

#include "renderedEntity.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "ground.hpp"

#include "lightManager.hpp"

#include "entityGroup.hpp"

#include <list>
#include <unordered_map>

class Game;


class World : public Resources::Scene
{
private:

    Game& game;

    EntityGroup entityGroup;

    Entity::BasicEntity* editorSelectedEntity = nullptr;

    /////////// UI ////////////////
    bool isPauseMenuOpen {false};
    bool inGame {true};
    bool gWin {false};
    int crosshairWidth = 0;
    int crosshairHeight = 0;
    GLuint crosshair = 0;
    int crossWidth = 0;
    int crossHeight = 0;
    GLuint cross = 0;
    float t = 0;
    //////////////////////////////

    float playTime = 0.f;

    // Searching if a file exists is expensive,
    // so at the start and each time we start the game,
    // we modify this value to know when we open the menu.
    bool isLoadAvailable = false; 

    bool isLoaded = false;

    bool isEditorMode = false;
    bool wasEditorKeyPressed = false;

    bool isPKeyPressed = false;

public:
    World(Game& game, bool isLoaded, bool isEditorMode);
    ~World();

    void setLowQualitySettings();
    void setHighQualitySettings();
    void switchQualitySettings();

    void makeNewLevel();

    void load()     override;
    void inputs()   override;
    void update()   override;
    void renderUI() override;
    void render()   override;
    void init();

    void updatePhysics();
    void updateCameraProjection();
    
    void pauseMenu();
    void hud();
    void gameWin();
    void gameOver();

};

#endif