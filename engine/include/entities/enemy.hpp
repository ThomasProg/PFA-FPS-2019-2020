#ifndef _ENEMY_HPP_
#define _ENEMY_HPP_

#include "entityID.hpp"
#include "basicEntity.hpp"

#include "saveInterface.hpp"

struct Vec3;

namespace Core
{
    class Engine;
}

namespace Entity 
{
    struct EnemyState
    {
        enum E_State
        {
            E_IDLE,
            E_PATROLLING,
            E_CHASING,
            E_FALLING,
            E_DEAD
        };

        public:
            E_State enemyState = E_IDLE;
            
    };

    class Enemy: public BasicEntity
    {
    private:
        static constexpr float epsilonReturnPatrolDistanceToPoint = 0.1f;
        static constexpr float maxSpeed = 0.02f;

        float angle = 0.f;
        float speed = 2.f; 

        bool  isDead = false;
        float timeLeftTillRespawn = 0.f;
        static constexpr float respawnCooldown = 5.f;

    public:
        EnemyState state;
        float detectionRadius = 10.0f;
        float patrolRadius = 5.f;
        Core::Maths::Vec3 position;
        Core::Maths::Vec3 patrolTarget;
        Core::Maths::Vec3 chaseTarget;

        inline virtual void raycastCollide() override;
        
        inline void setup2(const Core::Maths::Vec3& patrolTarget, const Core::Maths::Vec3& chaseTarget);

        inline void setup(Renderer::RendererSystem& renderer, 
                    const Resources::Model* model, 
                    const Resources::Shader* shader,
                    const Resources::Texture* texture,
                    Physics::TransformGraph& transformParent);
        void update(const Core::Engine& engine);
        void move(const Core::Engine& engine);
        void patrol(const Core::Engine& engine);
        bool isPlayerInRange() const;
        void chase(const Core::Engine& engine);

        void kill();
        // ~Enemy() {};

        void save(Save::Saver& saver)       override;
        void loadData(Save::Loader& loader) override;
    };
}

#include "enemy.inl"

#endif