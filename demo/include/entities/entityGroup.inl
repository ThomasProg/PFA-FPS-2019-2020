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

    player->camera.transform.transform.location.y = 1.f;

    if (controller == nullptr)
        controller = player.get();
    if  (camera == nullptr)
        camera = &player->camera;

    return player.get();
}

template<class... ARGS>
Entity::Ground* EntityGroup::addGround(const Physics::Transform& transform, ARGS&&... groundArgs)
{
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
Entity::Decoration* EntityGroup::addDecoration(const Physics::Transform& transform, 
                                               E_Model model, 
                                               E_Shader shader,
                                               const Core::Maths::Vec4& color, 
                                               ARGS&&... decoArgs)
{
    decorations.emplace_back(std::make_unique<Entity::Decoration>(std::forward<ARGS>(decoArgs)...));
    Entity::Decoration* deco = decorations.back().get();

    deco->setResources(engine.resourceManager, model, shader);
    deco->mesh.color = color;
    deco->setTransformParent(root);            
    deco->setTransform(transform);

    deco->addRendering(engine.rendererSystem);
    return deco;
}

template<class... ARGS>
Entity::Decoration* EntityGroup::addTree(const Physics::Transform& transform, ARGS&&... decoArgs)
{
    Entity::Decoration* tree = addDecoration(transform, 
                                             E_Model::E_TREE, 
                                             E_Shader::E_LIGHTED_FLATCOLOR,
                                             Core::Maths::Vec4{0.1f, 0.450f, 0.1f,1}, 
                                             std::forward<ARGS>(decoArgs)...);

    return tree;
}

template<class... ARGS>
Entity::Decoration* EntityGroup::addRock(const Physics::Transform& transform, ARGS&&... decoArgs)
{
    Entity::Decoration* rock = addDecoration(transform, 
                                             E_Model::E_ROCK2, 
                                             E_Shader::E_LIGHTED_FLATCOLOR,
                                             Core::Maths::Vec4{0.4f, 0.4f, 0.4f,1}, 
                                             std::forward<ARGS>(decoArgs)...);

    return rock;
}

template<class... ARGS>
Entity::Decoration* EntityGroup::addFirefly(const Physics::Transform& transform, ARGS&&... decoArgs)
{
    Entity::Decoration* rock = addDecoration(transform, 
                                             E_Model::E_ROCK2, 
                                             E_Shader::E_FLAT,
                                             Core::Maths::Vec4{0.952f, 1.f, 0.38f,1}, 
                                             std::forward<ARGS>(decoArgs)...);

    return rock;
}

template<class... ARGS>
Entity::Enemy* EntityGroup::addEnemy(const Physics::Transform& transform, ARGS&&... enemyArgs)
{ 
    enemies.emplace_back(std::make_unique<Entity::Enemy>(std::forward<ARGS>(enemyArgs)...));
    Entity::Enemy* enemy = enemies.back().get();
    enemy->life = 10;

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
Entity::Enemy* EntityGroup::addEnemyBoss(const Physics::Transform& transform, ARGS&&... enemyArgs)
{ 
    enemies.emplace_back(std::make_unique<Entity::Enemy>(std::forward<ARGS>(enemyArgs)...));
    Entity::Enemy* enemy = enemies.back().get();
    enemy->physicComp.collider.worldCollider.radius = 5.0f;
    enemy->type.enemyType = Entity::EnemyType::E_BOSS;
    enemy->life = 100;
    enemy->speed = 6.0f;

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




