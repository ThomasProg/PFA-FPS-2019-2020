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
#include <limits>

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
                         public Physics::CollisionComponentInterface<Physics::Shapes::Box>,
                         public Physics::PhysicComponentInterface,
                         public Controller::ControllerInterface
    {
    private: 
        //which model to use for the gun
        class Gun : public Physics::TransformInterface,
                    public Renderer::RenderableInterface
        {
        private:
            static constexpr Physics::Transform highQualityTransform = {{1.4f,-0.5f,0.f}, {0.f,0.f,0.f}, {1,1,1}};
            static constexpr Physics::Transform lowQualityTransform = {{2, -1, 1}, {0.f,0.f,0.f}, {0.4, 0.4, 5}};

        public:
            Gun();

            void setHighQuality(const DemoResourceManager& resourceManager);
            void setLowQuality(const DemoResourceManager& resourceManager);
        };

    public:
        Gun gun;

    private:
        static constexpr bool isMouseEnabled = true;
        static constexpr float mouseSensibility = 10.f;
        static constexpr float rotationSpeedOnKey = 3.f;
        static constexpr float rotationSpeed = 0.1;

        static constexpr float movementSpeed  = 10.f * 1.f;
        static constexpr float jumpSpeed      = 7.0f;
        static constexpr float jumpCoyoteTime = 0.1f;

        static constexpr float shootRayLength = 100.f;
        static constexpr float shootBulletScaleOnWall = 0.1f;  
        static constexpr float shootBulletScaleOnEntity = 0.05f;  

        static constexpr E_Model   defaultModel   = E_Model::E_GUN;
        static constexpr E_Texture defaultTexture = E_Texture::E_GUN;
        static constexpr E_Shader  defaultShader  = E_Shader::E_LIGHTED;
        static constexpr E_Audio   defaultShootSound = E_Audio::E_SHOOT;

    public:
        float lifePoints    = 10.f;
        float maxLifePoints = 10.f;
        unsigned int maxNbBullet = 10;
        unsigned int nbBullet = maxNbBullet;

        float shootCooldown = 0.3f;
        float lastShootTime = - shootCooldown;
        float lastJumpPressTime = std::numeric_limits<float>::min();
        
        Renderer::FPSCamera camera;

        std::function<void()> onPlayerDeath = nullptr;

        PlayerState state;

        //// UI ///////
        bool gOver {false};

        //// Audio ////
        Resources::AudioSource audio;


        CollisionComponentInterface<Physics::Shapes::Box>* currentGround = nullptr;

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

        inline void physicCompOnCollisionEnter(const Physics::Shapes::SegmentHit& hit, CollisionComponentInterface<Physics::Shapes::Box>* otherCollider) override;

        inline void physicCompOnCollisionExit(CollisionComponentInterface<Physics::Shapes::Box>* otherCollider) override;
    };
}

#include "player.inl"

#endif