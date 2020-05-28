#include "entityGroup.hpp"

template<class... ARGS>
Renderer::Light& EntityGroup::addLight(ARGS&&... lightArgs)
{
    lightManager.lights.emplace_back(std::forward<ARGS>(lightArgs)...);
    return lightManager.lights.back();
}

template<class... ARGS>
Entity::Player* EntityGroup::addPlayer(const Physics::Transform& transform, ARGS&&... playerArgs)
{
    player = std::make_unique<Entity::Player>(std::forward<ARGS>(playerArgs)...);
    if (player == nullptr)
        return nullptr;

    player->setTransformParent(root);
    player->setResources(engine.resourceManager);

    player->addRendering(engine.rendererSystem);
    player->addCollisions(engine.physicsSystem);
    player->addPhysics(engine.physicsSystem);

    player->camera.attachTo(player->transform);
    player->setTransform(transform);

    player->camera.transform.transform.location.y = 2.f;

    if (controller == nullptr)
        controller = player.get();
    if  (camera == nullptr)
        camera = &player->camera;

    return player.get();
}

template<class... ARGS>
Entity::Ground* EntityGroup::addGround(const Physics::Transform& transform, ARGS&&... groundArgs)
{
    // Entity::Ground* ground = new Entity::Ground(std::forward<ARGS>(groundArgs)...);
    // if (ground == nullptr)
    //     return nullptr;

    grounds.emplace_back(std::make_unique<Entity::Ground>(std::forward<ARGS>(groundArgs)...));
    Entity::Ground* ground = grounds.back().get();

    ground->setResources(engine.resourceManager);
    ground->setTransformParent(root);            
    ground->setTransform(transform);

    ground->addRendering(engine.rendererSystem);
    ground->addCollisions(engine.physicsSystem);
    return ground;
}

template<class... ARGS>
Entity::Enemy* EntityGroup::addEnemy(const Physics::Transform& transform, ARGS&&... enemyArgs)
{ 
    // Entity::Enemy* enemy = new Entity::Enemy();
    // if (enemy == nullptr)
    //     return nullptr;

    enemies.emplace_back(std::make_unique<Entity::Enemy>(std::forward<ARGS>(enemyArgs)...));
    Entity::Enemy* enemy = enemies.back().get();

    enemy->setResources(engine.resourceManager);
    enemy->setTransformParent(root);            
    enemy->setTransform(transform);

    enemy->addRendering(engine.rendererSystem);
    enemy->addCollisions(engine.physicsSystem);
    enemy->addPhysics(engine.physicsSystem);

    enemy->patrolTarget = transform.location;
    return enemy;
}

template<class... ARGS>
std::unique_ptr<Entity::RenderedEntity>& EntityGroup::addBullet(const Physics::Transform& transform, ARGS&&... bulletArgs)
{
    bullets.emplace_back(std::make_unique<Entity::RenderedEntity>(std::forward<ARGS>(bulletArgs)...));

    std::unique_ptr<Entity::RenderedEntity>& bullet = bullets.back();

    bullet->setResources(engine.resourceManager);
    bullet->addRendering(engine.rendererSystem);

    bullet->setTransformParent(root);
    bullet->setTransform(transform);
    bullet->timer = engine.lastTime + bullet->lifeTime;
    return bullet;
}




