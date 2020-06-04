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
    int bulletWidth = 0;
    int bulletHeight = 0;
    GLuint bullet = 0;
    // Time in seconds after the game is finished (lose or victory).
    float timeAfterGameIsFinished = 0;
    //////////////////////////////

    // Time since the world has been loaded in seconds.
    // Stops when the game is paused.
    float playTime = 0.f;

    // True if P is pressed
    bool isPKeyPressed = false;

public:
    World(Game& game);
    ~World() = default;

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