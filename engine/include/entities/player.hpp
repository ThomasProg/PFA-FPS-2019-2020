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

#include "audioSource.hpp"

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
                        // public Renderer::RenderableInterface,
                         public Physics::CollisionComponentInterface<Physics::Shapes::Box>,
                         public Physics::PhysicComponentInterface,
                         public Controller::ControllerInterface
    {
    private: 
        class Gun : public Physics::TransformInterface,
                    public Renderer::RenderableInterface
        {
        public:
            Gun() 
                : Renderer::RenderableInterface(&transform)
            {}
        };

        Gun gun;

    private:
        static constexpr float movementSpeed  = 10.f * 1.f;
        static constexpr float jumpSpeed      = 7.0f;
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

        //// Audio ////
        Resources::AudioSource audio;


        bool canJump = false;

        // ====== Input Keys ====== //
        enum E_Inputs : unsigned int
        {
            E_FORWARD = 0,
            E_BACKWARD,
            E_RIGHT,
            E_LEFT,
            E_JUMP,
            E_FIRE,
            E_LAST
        };
        static constexpr unsigned int nbInputKeys = static_cast<unsigned int> (E_Inputs::E_LAST);

        static std::array<bool, nbInputKeys> getDownKeysAzertyAndQwery(const Core::Engine& engine);

        std::array<bool, nbInputKeys> (*getDownKeys) (const Core::Engine& engine) = getDownKeysAzertyAndQwery;

    public:
        
        Player();

        inline void addRendering(Renderer::RendererSystem& rendererSystem);
        inline void removeRendering(Renderer::RendererSystem& rendererSystem);

        void setTransformParent(Physics::TransformGraph& transformParent);
        void setTransform(const Physics::Transform& newTransform);

        void inputs(const Core::Engine& engine) override;

        void tryToJump(const Core::Engine& engine);
        void setResources(const DemoResourceManager&);

        Physics::Shapes::Segment3D getShootRay() const;
        void shoot(Physics::PhysicsSystem& physicsSystem, EntityGroup& entityGroup, float playTime);

        void dealDamages(float damages);

        void reloadAmmo();

        void physicCompOnCollisionEnter(const Physics::Shapes::SegmentHit& hit, CollisionComponentInterface<Physics::Shapes::Box>* otherCollider) override 
        {
            if (hit.normal.y > 0.5) canJump = true;
            // std::cout << hit.normal << std::endl;
            // std::cout << "Enter collision" << std::endl;
        }

        void physicCompOnCollisionExit(CollisionComponentInterface<Physics::Shapes::Box>* otherCollider) override 
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

#include "player.inl"

#endif