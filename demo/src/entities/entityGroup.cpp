#include "entityGroup.hpp"

EntityGroup::EntityGroup(Core::Engine& engine)
    : engine(engine)
{
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