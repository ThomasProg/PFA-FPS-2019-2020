#ifndef _ENEMY_HPP_
#define _ENEMY_HPP_

#include "transformInterface.hpp"
#include "collisionComponentInterface.hpp"
#include "physicComponentInterface.hpp"
#include "renderableInterface.hpp"
#include "saveInterface.hpp"

#include "saveInterface.hpp"

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
            E_State enemyState = E_IDLE;
            
    };

    class Enemy final : public Physics::TransformInterface,
                  public Physics::CollisionComponentInterface<Box>, 
                  public Physics::PhysicComponentInterface, 
                  public Renderer::RenderableInterface,
                  public Save::SaveInterface
    {
    private:
        static constexpr float epsilonReturnPatrolDistanceToPoint = 0.5f;
        // static constexpr float maxSpeed = 0.07f;
        static constexpr float maxSpeed = 5.f;

        float angle = 0.f;
        float speed = 2.f; 

        float timeLeftTillRespawn = 0.f;
        static constexpr float respawnCooldown = 4.f;
        int maxLife = 10;
        int life = 10;
        float lastReceivedHitTime = -1.f;

    public:
        float attackCooldown = 1.f;
        float lastAttackTime = -attackCooldown;

    public:
        EnemyState state;
        float detectionRadius = 10.0f;
        float attackRadius    = 2.f;
        float patrolRadius    = 5.f;
        Core::Maths::Vec3 position;
        Core::Maths::Vec3 patrolTarget = {0.f, 0.f, 0.f};
        Core::Maths::Vec3 chaseTarget = {0.f,0.f,0.f};

        Entity::Player* target = nullptr;
        
        Enemy() 
            : Physics::CollisionComponentInterface<Box>(&transform),
              Physics::PhysicComponentInterface(&transform),
              Renderer::RenderableInterface(&transform)
        {
            collider.isOverlap = true;
            physicComp.collider.worldCollider.radius = 1.f;
        }

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
        // ~Enemy() {};

        void save(Save::Saver& saver)       override {}
        void loadData(Save::Loader& loader) override {}


        // virtual void onCollisionEnter        (const SegmentHit& hit) override;
        // virtual void onCollisionExit         () override;
        // virtual void onOverlapEnterSelfHit   (const SegmentHit& hit) override;
        // virtual void onOverlapEnterAnotherHit(const SegmentHit& hit) override;
        virtual void physicCompOnOverlapEnter   (const Physics::PhysicsSystem::CollisionsCallbacksSentData& data) override
        {
            if (data.encounteredEntityID == this)
                return;

            if (data.hit.normal.y < -0.5)
            {
                kill();
            }
        }

        virtual void colliderOnOverlapEnter   (const Physics::PhysicsSystem::CollisionsCallbacksSentData& data) override
        {
            if (data.movingEntityID == this)
                return;

            if (data.hit.normal.y > 0.5)
            {
                kill();
            }
        }
    };
}

#include "enemy.inl"

#endif