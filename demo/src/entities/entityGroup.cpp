#include "entityGroup.hpp"

EntityGroup::EntityGroup(Core::Engine& engine)
    : engine(engine)
{

}

EntityGroup::~EntityGroup()
{
    engine.rendererSystem.erase(player->meshIt);
    engine.physicsSystem.erase(player->colliderIt);
    engine.physicsSystem.erase(player->physicCompIt);

    for (std::unique_ptr<Entity::Ground>& ground : grounds)
    {
        // if (ground)
        {
            engine.rendererSystem.erase(ground->meshIt);
            engine.physicsSystem.erase(ground->colliderIt);
        }
    }

    for (std::unique_ptr<Entity::Enemy>& enemy : enemies)
    {
        // if (enemy)
        {
            engine.rendererSystem.erase(enemy->meshIt);
            engine.physicsSystem.erase(enemy->colliderIt);
            engine.physicsSystem.erase(enemy->physicCompIt);
        }
    }

    for (std::unique_ptr<Entity::RenderedEntity>& bullet : bullets)
    {
        engine.rendererSystem.erase(bullet->meshIt);
    }


    player = nullptr;
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

void EntityGroup::removeEnemyFromSytems(Entity::Enemy* enemy)
{
    engine.rendererSystem.erase(enemy->meshIt);
    engine.physicsSystem.erase(enemy->colliderIt);
    engine.physicsSystem.erase(enemy->physicCompIt);
}


void EntityGroup::removeEnemy(unsigned int index)
{
    enemies[index] = std::move(enemies.back());
    enemies.pop_back();
}

void EntityGroup::collectGarbage()
{
    for (uint i = 0; i < enemies.size(); i++)
    {
        if (enemies[i]->state.enemyState == Entity::EnemyState::E_DEAD)
        {
            removeEnemyFromSytems(enemies[i].get());
            removeEnemy(i);
        }
    }
}