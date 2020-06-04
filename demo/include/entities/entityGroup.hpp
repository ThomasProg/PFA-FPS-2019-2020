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
    // All the entities will be attached to the root once they are added.
    Physics::TransformGraph root;

    // ====== Entities ====== //

    // Contains lights.
    Renderer::LightManager lightManager {10};
    std::unique_ptr<Entity::Player> player = nullptr;
    // Contains grounds, walls, etc.
    std::vector<std::unique_ptr<Entity::Ground>> grounds;
    // Contains Enemies (including Boss).
    std::vector<std::unique_ptr<Entity::Enemy>> enemies;
    // Contains trees, rocks, etc.
    std::vector<std::unique_ptr<Entity::Decoration>> decorations;

    // Contains the bullets.
    std::vector<std::unique_ptr<Entity::RenderedEntity>> bullets;

    // Points toward current player controller.
    Controller::ControllerInterface* controller = nullptr;
    // Points toward current camera used.
    Renderer::Camera* camera = nullptr;

    EntityGroup(Core::Engine&);

    ~EntityGroup();

    // Not returning a reference or a pointer;
    // Modifying lights sets a dirty flag, 
    // we do not want the reference to be used too long, 
    // or the dirty flag would become pointless.
    template<class... ARGS>
    void addLight (ARGS&&... lightArgs);
    // Remove all lights from the EntityGroup.
    inline void removeAllLights();

    // Adds a player to the group,
    // attach it to systems,
    // and use its camera and controller if there is currently no one selected.
    template<class... ARGS>
    Entity::Player* addPlayer(const Physics::Transform& transform, ARGS&&... playerArgs);

    // Adds a ground to the group
    // and attach it to systems.
    template<class... ARGS>
    Entity::Ground* addGround(const Physics::Transform& transform, ARGS&&... groundArgs);

    // Adds a decoration to the group
    // and attach it to systems.
    template<class... ARGS>
    Entity::Decoration* addDecoration(const Physics::Transform& transform, 
                                      E_Model model, 
                                      E_Shader shader,
                                      const Core::Maths::Vec4& color, 
                                      ARGS&&... groundArgs);
    // Detach all decorations from systems, 
    // and remove them.
    void removeAllDecorations();

    // Adds a Tree, which is a decoration, to the group,
    // and attach it to systems.
    // This calls addDecoration().
    template<class... ARGS>
    Entity::Decoration* addTree(const Physics::Transform& transform, ARGS&&... decoArgs);

    // Adds a Rock, which is a decoration, to the group,
    // and attach it to systems.
    // This calls addDecoration().
    template<class... ARGS>
    Entity::Decoration* addRock(const Physics::Transform& transform, ARGS&&... decoArgs);

    // Adds a Firefly, which is a decoration, to the group,
    // and attach it to systems.
    // This calls addDecoration().
    template<class... ARGS>
    Entity::Decoration* addFirefly(const Physics::Transform& transform, ARGS&&... decoArgs);

    // Adds a Enemy to the group
    // and attach it to systems.
    template<class... ARGS>
    Entity::Enemy* addEnemy (const Physics::Transform& transform, ARGS&&... enemyArgs);

    // Adds a EnemyBoss, which is an Enemy, to the group,
    // and attach it to systems.
    template<class... ARGS>
    Entity::Enemy* addEnemyBoss (const Physics::Transform& transform, ARGS&&... enemyArgs);

    // Removes the enemy at the given index from "enemies" array.
    // It is also removed from systems.
    // The index should be valid.
    void removeEnemy(unsigned int index);
    // Removes the enemy from systems.
    // The pointer should be valid.
    void removeEnemyFromSytems(Entity::Enemy& enemy);

    // Adds a Bullet to the group
    // and attach it to systems.
    template<class... ARGS>
    std::unique_ptr<Entity::RenderedEntity>& addBullet(const Physics::Transform& transform, ARGS&&... bulletArgs);

    // Removes dead entities (enemies).
    void collectGarbage();
};

#include "entityGroup.inl"

#endif