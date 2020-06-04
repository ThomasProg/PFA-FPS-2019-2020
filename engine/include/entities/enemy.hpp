#ifndef _ENEMY_HPP_
#define _ENEMY_HPP_

#include "transformInterface.hpp"
#include "collisionComponentInterface.hpp"
#include "physicComponentInterface.hpp"
#include "renderableInterface.hpp"
#include "saveInterface.hpp"

#include "saveInterface.hpp"

#include "audioSource.hpp"

struct Vec3;

namespace Core
{
    class Engine;
}

namespace Entity 
{
    struct Player;

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
            E_State enemyState = E_PATROLLING;
            
    };

    struct EnemyType
    {
        enum E_Type
        {
            E_NORMAL,
            E_BOSS
        };

        public:
            E_Type enemyType = E_NORMAL;
    };
    
    class Enemy final : public Physics::TransformInterface,
                  public Physics::CollisionComponentInterface<Physics::Shapes::Box>, 
                  public Physics::PhysicComponentInterface, 
                  public Renderer::RenderableInterface
    {
    private:
        static constexpr float epsilonReturnPatrolDistanceToPoint = 0.5f;
        static constexpr float maxSpeed = 7.5f;

        float angle = 0.f;

        float timeLeftTillRespawn = 0.f;
        static constexpr float respawnCooldown = 4.f;
        int maxLife = 10;
        float lastReceivedHitTime = -1.f;
        float timeBetweenHitBoss = 2.0f;
        float timeForHit = 0.0f;

    public:
        float attackCooldown = 1.f;
        float lastAttackTime = -attackCooldown;

    public:
        int life = 10;
        float speed = 3.f; 
        EnemyState state;
        EnemyType type;
        float detectionRadius = 20.0f;
        float attackRadius    = 2.f;
        float patrolRadius    = 5.f;
        Core::Maths::Vec3 position;
        Core::Maths::Vec3 patrolTarget = {0.f, 0.f, 0.f};
        Core::Maths::Vec3 chaseTarget = {0.f,0.f,0.f};

        Entity::Player* target = nullptr;

        //// Audio ////
        Resources::AudioSource attackSound;
        
        Enemy();

        ~Enemy() = default;
        
        void update(const Core::Engine& engine, float playTime);
        void move(const Core::Engine& engine);
        void patrol(const Core::Engine& engine);
        bool isTargetInChaseRange() const;
        bool isTargetInAttackRange() const;
        void chase(const Core::Engine& engine);

        void setResources(const DemoResourceManager&);

        void takeDamage(int damage, float playTime);
        void tryToAttack(float playTime);
        void tryToRespawn();

        void lerpColorBackToNormal(float playTime);

        void kill();

        virtual void physicCompOnOverlapEnter   (const Physics::PhysicsSystem::CollisionsCallbacksSentData& data) override;

        virtual void colliderOnOverlapEnter     (const Physics::PhysicsSystem::CollisionsCallbacksSentData& data) override;
    };
}

#endif