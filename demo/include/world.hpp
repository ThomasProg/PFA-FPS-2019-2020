#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "scene.hpp"

#include "enumedResourceManager.hpp"
#include "transformGraph.hpp"
#include "physicsSystem.hpp"
#include "saveSystem.hpp"

#include "mesh.hpp"

#include "fpsCamera.hpp"
#include "tpsCamera.hpp"

#include "basicEntity.hpp"
#include "player.hpp"
#include "enemy.hpp"

#include <list>

class Game;

// @Obsolete
class EditorMode
{
public:
    using EnumType = unsigned char;

    enum : EnumType
    {
        E_TRANSLATION = 0,
        E_ROTATION_X,
        E_ROTATION_Y,
        E_ROTATION_Z,
        E_SCALE_X,
        E_SCALE_Y,
        E_SCALE_Z,
        E_LAST
    } mode;

    void next()
    {
        mode = static_cast<decltype(mode)>((static_cast<EnumType>(mode) + 1) % E_LAST); 
    }

    void nextRotation()
    {
        mode = static_cast<decltype(mode)>(static_cast<EnumType>(mode) + 1); 
        if (static_cast<EnumType>(mode) > E_ROTATION_Z || static_cast<EnumType>(mode) < E_ROTATION_X)
            mode = E_ROTATION_X;
    }

    void nextScale()
    {
        mode = static_cast<decltype(mode)>(static_cast<EnumType>(mode) + 1); 
        if (static_cast<EnumType>(mode) > E_SCALE_Z || static_cast<EnumType>(mode) < E_SCALE_X)
            mode = E_SCALE_X;
    }
};



class World : public Resources::Scene, public Save::SaveInterface
{
private:
    Game& game;

    Save::SaveSystem saveSystem;
    Physics::TransformGraph root; 

    Entity::Player player;
    Entity::BasicEntity sphere2;
    std::list<Entity::BasicEntity> grounds;
    std::list<Entity::Enemy> enemies;
    // Entity::BasicEntity ground;
    // Entity::BasicEntity ground2;
    // Entity::BasicEntity ground3;
    Entity::BasicEntity dog;

    // Entity::Enemy enemy;

    Renderer::TPSCamera tpsCamera;
    Renderer::FPSCamera fpsCamera;

    Controller::ControllerInterface* controller = &player;
    Renderer::Camera* camera = &player.camera;

    Entity::BasicEntity* editorSelectedEntity = nullptr;

    bool isPauseMenuOpen {false};
    bool inGame {true};

    // Searching if a file exists is expensive,
    // so at the start and each time we start the game,
    // we modify this value to know when we open the menu.
    bool isLoadAvailable = false; 

    bool isEditorMode = false;
    bool wasEditorKeyPressed = false;
    EditorMode editorMode {EditorMode::E_TRANSLATION};

public:
    World(Game& game, bool isLoaded, bool isEditorMode);
    ~World();
    void inputs() override;
    void update() override;
    void renderUI() override;
    void render() override;
    void init();

    void updatePhysics();
    void updateEditorFunctions();
    void updateCameraProjection();
    
    void pauseMenu();
    void hud();
    void gameOver();

    void addGround(const Core::Maths::Vec3&);
    void addEnemy(const Core::Maths::Vec3&);

    void save(Save::Saver& saver) override;
    void loadData(Save::Loader& loader) override;
};

#endif