#include "entityGroup.hpp"

EntityGroup::EntityGroup(Core::Engine& engine)
    : engine(engine)
{
    lightManager.lightsBufferInit(10);
    lightManager.lights.emplace_back();
    lightManager.lights.emplace_back();
    {
        Renderer::LightData& l = lightManager.lights[lightManager.lights.size() - 1].lightData;
        l.location = {20.f, -27.f, 12, 0.0}; 
        l.lightType = 3;
        l.ambient = {0.7, 0.7,0.7,0};
    }
}

EntityGroup::~EntityGroup()
{
    engine.rendererSystem.erase(player->meshIt);

    if (player != nullptr)
        delete player;

    for (Entity::Enemy* enemy : enemies)
    {
        if (enemy != nullptr)
        {
            engine.rendererSystem.erase(enemy->meshIt);
            delete enemy;
        }
    }

    for (Entity::Ground* ground : grounds)
    {
        if (ground != nullptr)
        {
            engine.rendererSystem.erase(ground->meshIt);
            delete ground;
        }
    }

    for (std::unique_ptr<Entity::RenderedEntity>& bullet : bullets)
    {
        engine.rendererSystem.erase(bullet->meshIt);
    }


    player = nullptr;
    enemies.clear();
    grounds.clear();
}

void EntityGroup::addPlayer(const Physics::Transform& transform)
{
    if (player != nullptr)
        delete player;

    player = new Entity::Player();
    player->setTransformParent(root);
    player->setResources(engine.resourceManager);

    player->addRendering(engine.rendererSystem);
    player->addCollisions(engine.physicsSystem);
    player->addPhysics(engine.physicsSystem);

    player->camera.attachTo(player->transform);
    player->setTransform(transform);

    player->camera.transform.transform.location.y = 2.f;

    if (controller == nullptr)
        controller = player;
    if  (camera == nullptr)
        camera = &player->camera;
}

void EntityGroup::addGround(const Physics::Transform& transform)
{
    Entity::Ground* ground = new Entity::Ground();
    grounds.emplace_back(ground);

    ground->setResources(engine.resourceManager);
    ground->setTransformParent(root);            
    ground->setTransform(transform);

    ground->addRendering(engine.rendererSystem);
    ground->addCollisions(engine.physicsSystem);
}

void EntityGroup::addEnemy(const Physics::Transform& transform)
{ 
    Entity::Enemy* enemy = new Entity::Enemy();
    enemies.emplace_back(enemy);

    enemy->setResources(engine.resourceManager);
    enemy->setTransformParent(root);            
    enemy->setTransform(transform);

    enemy->addRendering(engine.rendererSystem);
    enemy->addCollisions(engine.physicsSystem);
    enemy->addPhysics(engine.physicsSystem);

    enemy->patrolTarget = transform.location;
}

void EntityGroup::addBullet(const Physics::Transform& transform)
{
    bullets.emplace_back(std::make_unique<Entity::RenderedEntity>());

    std::unique_ptr<Entity::RenderedEntity>& bullet = bullets.back();

    bullet->setResources(engine.resourceManager);
    bullet->addRendering(engine.rendererSystem);

    bullet->setTransformParent(root);
    bullet->setTransform(transform);
    bullet->timer = engine.lastTime + bullet->lifeTime;
}

void EntityGroup::setKeys(bool isAzerty)
{
    if (isAzerty)
    {
        player->inputKeys.keys = 
        {
            GLFW_KEY_Z,
            GLFW_KEY_S,
            GLFW_KEY_D,
            GLFW_KEY_Q,
            GLFW_KEY_SPACE
        };

        // fpsCamera.inputKeys.keys = 
        // {
        //     GLFW_KEY_Z,
        //     GLFW_KEY_S,
        //     GLFW_KEY_D,
        //     GLFW_KEY_Q,
        // };
    }
    else 
    {
        player->inputKeys.keys = 
        {
            GLFW_KEY_W,
            GLFW_KEY_S,
            GLFW_KEY_D,
            GLFW_KEY_A,
            GLFW_KEY_SPACE
        };

        // fpsCamera.inputKeys.keys = 
        // {
        //     GLFW_KEY_W,
        //     GLFW_KEY_S,
        //     GLFW_KEY_D,
        //     GLFW_KEY_A,
        // };
    }
}