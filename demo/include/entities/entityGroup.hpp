#ifndef _ENTITY_GROUP_HPP_
#define _ENTITY_GROUP_HPP_

#include "transformGraph.hpp"
#include "lightManager.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "renderedEntity.hpp"
#include "ground.hpp"

#include <vector>

namespace Core
{
    struct Engine;
}

class EntityGroup
{
private:


public:
    Core::Engine& engine;
    Physics::TransformGraph root;

    Renderer::LightManager lightManager;

    // Save::SaveSystem saveSystem;

    Entity::Player* player = nullptr;
    // std::list<Entity::BasicEntity> grounds;
    // std::list<Entity::Enemy> enemies;
    // std::vector<Entity::BasicEntity*> grounds;
    std::vector<Entity::Ground*> grounds;
    std::vector<Entity::Enemy*> enemies;
    // std::unordered_map<Entity::EntityID, Entity::BasicEntity> grounds;
    // std::unordered_map<Entity::EntityID, Entity::Enemy> enemies;

    std::vector<std::unique_ptr<Entity::RenderedEntity>> bullets;

    // Entity::Enemy enemy;

    // Renderer::TPSCamera tpsCamera;
    // Renderer::FPSCamera fpsCamera;

    Controller::ControllerInterface* controller = nullptr;
    Renderer::Camera* camera = nullptr;

    EntityGroup(Core::Engine&);

    ~EntityGroup();

    void addPlayer();
    void addGround(const Physics::Transform& transform);
    void addEnemy(const Physics::Transform& transform);
    void addBullet(const Physics::Transform& transform);

    void setKeys(bool isAzerty);
};

#endif