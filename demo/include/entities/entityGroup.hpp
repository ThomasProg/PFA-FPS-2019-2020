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

    Renderer::LightManager lightManager {30};

    // Save::SaveSystem saveSystem;

    Entity::Player* player = nullptr;
    std::vector<Entity::Ground*> grounds;
    std::vector<Entity::Enemy*> enemies;

    std::vector<std::unique_ptr<Entity::RenderedEntity>> bullets;

    // Renderer::TPSCamera tpsCamera;
    // Renderer::FPSCamera fpsCamera;

    Controller::ControllerInterface* controller = nullptr;
    Renderer::Camera* camera = nullptr;

    EntityGroup(Core::Engine&);

    ~EntityGroup();

    template<class... ARGS>
    Renderer::Light& addLight (ARGS&&... args);

    template<class... ARGS>
    Entity::Player* addPlayer(const Physics::Transform& transform, ARGS&&... args);

    template<class... ARGS>
    Entity::Ground* addGround(const Physics::Transform& transform, ARGS&&... args);

    template<class... ARGS>
    Entity::Enemy* addEnemy (const Physics::Transform& transform, ARGS&&... args);

    template<class... ARGS>
    std::unique_ptr<Entity::RenderedEntity>& addBullet(const Physics::Transform& transform, ARGS&&... args);

    void setKeys(bool isAzerty);
};

#include "entityGroup.inl"

#endif