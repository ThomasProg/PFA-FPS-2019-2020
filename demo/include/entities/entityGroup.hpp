#ifndef _ENTITY_GROUP_HPP_
#define _ENTITY_GROUP_HPP_

#include "transformGraph.hpp"
#include "lightManager.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "decoration.hpp"
#include "renderedEntity.hpp"
#include "ground.hpp"

#include <memory>
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

    std::unique_ptr<Entity::Player> player = nullptr;
    std::vector<std::unique_ptr<Entity::Ground>> grounds;
    std::vector<std::unique_ptr<Entity::Enemy>> enemies;
    std::vector<std::unique_ptr<Entity::Decoration>> decorations;

    std::vector<std::unique_ptr<Entity::RenderedEntity>> bullets;

    // Renderer::TPSCamera tpsCamera;
    // Renderer::FPSCamera fpsCamera;

    Controller::ControllerInterface* controller = nullptr;
    Renderer::Camera* camera = nullptr;

    EntityGroup(Core::Engine&);

    ~EntityGroup();

    template<class... ARGS>
    Renderer::Light& addLight (ARGS&&... lightArgs);

    template<class... ARGS>
    Entity::Player* addPlayer(const Physics::Transform& transform, ARGS&&... playerArgs);

    template<class... ARGS>
    Entity::Ground* addGround(const Physics::Transform& transform, ARGS&&... groundArgs);

    template<class... ARGS>
    Entity::Decoration* addDecoration(const Physics::Transform& transform, 
                                      E_Model model, 
                                      E_Shader shader,
                                      const Core::Maths::Vec4& color, 
                                      ARGS&&... groundArgs);

    template<class... ARGS>
    Entity::Decoration* addTree(const Physics::Transform& transform, ARGS&&... decoArgs);

    template<class... ARGS>
    Entity::Decoration* addRock(const Physics::Transform& transform, ARGS&&... decoArgs);

    template<class... ARGS>
    Entity::Decoration* addFirefly(const Physics::Transform& transform, ARGS&&... decoArgs);

    template<class... ARGS>
    Entity::Enemy* addEnemy (const Physics::Transform& transform, ARGS&&... enemyArgs);

    template<class... ARGS>
    Entity::Enemy* addEnemyBoss (const Physics::Transform& transform, ARGS&&... enemyArgs);

    void removeEnemy(unsigned int index);
    void removeEnemyFromSytems(Entity::Enemy* enemy);

    template<class... ARGS>
    std::unique_ptr<Entity::RenderedEntity>& addBullet(const Physics::Transform& transform, ARGS&&... bulletArgs);

    void setKeys(bool isAzerty);

    void collectGarbage();
};

#include "entityGroup.inl"

#endif