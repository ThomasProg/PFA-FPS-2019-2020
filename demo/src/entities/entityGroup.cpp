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
    for (Entity::Enemy* enemy : enemies)
        if (enemy != nullptr)
            delete enemy;

    for (Entity::Ground* ground : grounds)
        if (ground != nullptr)
            delete ground;


    enemies.clear();
    grounds.clear();
}

void EntityGroup::addPlayer()
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
    player->setTransform({{0, 2.f, 0}});

    if (controller == nullptr)
        controller = player;
    if  (camera == nullptr)
        camera = &player->camera;
}

void EntityGroup::addGround(const Physics::Transform& transform)
{
    grounds.emplace_back(new Entity::Ground());
    Entity::Ground* ground = grounds.back();

    ground->setResources(engine.resourceManager);
    ground->setTransformParent(root);            
    ground->setTransform(transform);

    ground->addRendering(engine.rendererSystem);
    ground->addCollisions(engine.physicsSystem);
}

void EntityGroup::addEnemy(const Physics::Transform& transform)
{ 
    enemies.emplace_back(new Entity::Enemy());
    Entity::Enemy* enemy = enemies.back();

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