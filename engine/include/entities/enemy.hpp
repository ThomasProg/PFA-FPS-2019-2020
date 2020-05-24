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
        static constexpr float epsilonReturnPatrolDistanceToPoint = 0.1f;
        // static constexpr float maxSpeed = 0.07f;
        static constexpr float maxSpeed = 0.02f;

        float angle = 0.f;
        float speed = 2.f; 

        bool  isDead = false;
        float timeLeftTillRespawn = 0.f;
        static constexpr float respawnCooldown = 5.f;
        int maxLife = 10;
        int life = 10;

    public:
        EnemyState state;
        float detectionRadius = 10.0f;
        float patrolRadius = 5.f;
        Core::Maths::Vec3 position;
        Core::Maths::Vec3 patrolTarget = {0.f, 0.f, 0.f};
        Core::Maths::Vec3 chaseTarget = {0.f,0.f,0.f};

        inline virtual void raycastCollide();
        
        Enemy() 
            : Physics::CollisionComponentInterface<Box>(&transform),
              Physics::PhysicComponentInterface(&transform),
              Renderer::RenderableInterface(&transform)
        {
            collider.isOverlap = true;
            physicComp.collider.worldCollider.radius = 1.f;
        }

        ~Enemy() = default;

        // inline void setup2(const Core::Maths::Vec3& patrolTarget, const Core::Maths::Vec3& chaseTarget);

        // inline void setup(Renderer::RendererSystem& renderer, 
        //             const Resources::Model* model, 
        //             const Resources::Shader* shader,
        //             const Resources::Texture* texture,
        //             Physics::TransformGraph& transformParent);
        
        void update(const Core::Engine& engine);
        void move(const Core::Engine& engine);
        void patrol(const Core::Engine& engine);
        bool isPlayerInRange() const;
        void chase(const Core::Engine& engine);

        void setResources(const DemoResourceManager&);

        void takeDamage(int damage);

        void kill();
        // ~Enemy() {};

        void save(Save::Saver& saver)       override;
        void loadData(Save::Loader& loader) override;


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