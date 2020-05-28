#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "rendererSystem.hpp"

#include "controllerInterface.hpp"
#include "transformInterface.hpp"
#include "renderableInterface.hpp"
#include "physicComponentInterface.hpp"
#include "collisionComponentInterface.hpp"

#include "tpsCamera.hpp"
#include "fpsCamera.hpp"

#include <array>

class EntityGroup;

namespace Core
{
    class Engine;
}

namespace Entity
{
    struct PlayerState
    {
    public:
        // Isn't enum class, because it already is limited to PlayerState Class
        enum EPlayerState : unsigned char
        {
            E_IDLE,
            E_WALKING,
            E_JUMPING,
            E_GLIDING,
            E_DEAD
        };

    public:
        EPlayerState playerState = E_JUMPING;

    public:
        bool isOnGround() const noexcept;
    };

    // Example Class for Player
    class Player final : public Physics::TransformInterface,
                         public Renderer::RenderableInterface,
                         public Physics::CollisionComponentInterface<Physics::Shapes::Box>,
                         public Physics::PhysicComponentInterface,
                         public Controller::ControllerInterface
    {
    private:
        static constexpr float movementSpeed  = 10.f * 1.f;
        static constexpr float jumpSpeed      = 10.0f;
        static constexpr float jumpCoyoteTime = 0.1f;
        static constexpr float shootRayLength = 100.f;


    public:
        float lifePoints    = 10.f;
        float maxLifePoints = 10.f;
        unsigned int maxNbBullet = 10;
        unsigned int nbBullet = maxNbBullet;

        float shootCooldown = 0.3f;
        float lastShootTime = - shootCooldown;

        float lastJumpPressTime = -123456789.f;
        
        // Renderer::TPSCamera camera;
        Renderer::FPSCamera camera;

        std::function<void()> onPlayerDeath = nullptr;
        PlayerState state;

        //// UI ///////
        bool gOver {false};

        // ====== Input Keys ====== //
    public:
        static constexpr size_t nbInputKeys = 5;

        union 
        {
            struct 
            {
                unsigned int forward  = GLFW_KEY_W,
                             backward = GLFW_KEY_S,
                             right    = GLFW_KEY_D,
                             left     = GLFW_KEY_A,
                             jump     = GLFW_KEY_SPACE,
                             fire     = GLFW_MOUSE_BUTTON_RIGHT;
            };

            std::array<unsigned int, nbInputKeys> keys;
        } inputKeys;
    public:
        using InputsKeys = std::array<unsigned int, nbInputKeys>;
        // ======================== //

    public:
        
        Player() 
            : Renderer::RenderableInterface(&transform),
              Physics::CollisionComponentInterface<Physics::Shapes::Box>(&transform),
              Physics::PhysicComponentInterface(&transform)
        {
            collider.isOverlap = true;
            physicComp.collider.worldCollider.radius = 1.f;
        }

        void inputs(const Core::Engine& engine) override;

        void tryToJump(const Core::Engine& engine);
        void setResources(const DemoResourceManager&);

        bool isShooting(const Core::Engine& engine) const;
        Physics::Shapes::Segment3D getShootRay() const;
        void shoot(Physics::PhysicsSystem& physicsSystem, EntityGroup& entityGroup, float playTime);

        void dealDamages(float damages);

        void reloadAmmo();

        void physicCompOnCollisionEnter        (const Physics::Shapes::SegmentHit&) override 
        {
            // std::cout << "Enter collision" << std::endl;
        }

        void physicCompOnCollisionExit        () override 
        {
            // std::cout << "Exit collision" << std::endl;
        }


        void physicCompOnOverlapEnter   (const Physics::PhysicsSystem::CollisionsCallbacksSentData& data) override
        {
            if (data.encounteredEntityID == this)
                return;

            // if (data.hit.normal.y < 0.5)
            // {
            //     dealDamages(1.f);
            // }
        }

        void colliderOnOverlapEnter   (const Physics::PhysicsSystem::CollisionsCallbacksSentData& data) override
        {
            if (data.movingEntityID == this)
                return;

            // if (data.hit.normal.y > - 0.5)
            // {
            //     dealDamages(1.f);
            // }
        }
    };
}

#endif