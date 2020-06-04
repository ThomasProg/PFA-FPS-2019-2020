#include "entityGroup.hpp"

EntityGroup::EntityGroup(Core::Engine& engine)
    : engine(engine)
{

}

EntityGroup::~EntityGroup()
{
    player->removeRendering(engine.rendererSystem);
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

    removeAllDecorations();

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

void EntityGroup::removeAllDecorations()
{
    for (std::unique_ptr<Entity::Decoration>& deco : decorations)
    {
        assert(deco);
        engine.rendererSystem.erase(deco->meshIt);
    }
    decorations.clear();
}

void EntityGroup::removeEnemyFromSytems(Entity::Enemy& enemy)
{
    engine.rendererSystem.erase(enemy.meshIt);
    engine.physicsSystem.erase(enemy.colliderIt);
    engine.physicsSystem.erase(enemy.physicCompIt);
}


void EntityGroup::removeEnemy(unsigned int index)
{
    assert(index < enemies.size());
    enemies[index] = std::move(enemies.back());
    enemies.pop_back();
}

void EntityGroup::collectGarbage()
{
    for (unsigned int i = 0; i < enemies.size(); i++)
    {
        if (enemies[i] && enemies[i]->state.enemyState == Entity::EnemyState::E_DEAD)
        {
            removeEnemyFromSytems(*enemies[i].get());
            removeEnemy(i);
        }
    }
}