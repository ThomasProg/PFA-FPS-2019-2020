#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "entityID.hpp"
#include "rendererSystem.hpp"
#include "resourceManager.hpp"

#include "engine.hpp"

#include "basicEntity.hpp"
#include "controllerInterface.hpp"
#include "tpsCamera.hpp"

#include <array>

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
        EPlayerState playerState = E_WALKING;

    public:
        bool isOnGround() const noexcept;
    };

    // Example Class for Player
    class Player : public BasicEntity, public Controller::ControllerInterface
    {
    private:
        static constexpr float movementSpeed = 10.f;
        static constexpr float jumpSpeed     = 0.1f;


    public:
        float lifePoints    = 10.f;
        float maxLifePoints = 10.f;
        Renderer::TPSCamera camera;

        std::function<void()> onPlayerDeath = nullptr;

    private:
        PlayerState state;

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
                             jump     = GLFW_KEY_SPACE;
            };

            std::array<unsigned int, nbInputKeys> keys;
        } inputKeys;
    public:
        using InputsKeys = std::array<unsigned int, nbInputKeys>;
        // ======================== //

    public:
        
        
        void setup(Renderer::RendererSystem& renderer, 
                    const Resources::Model* model, 
                    const Resources::Shader* shader, 
                    Physics::TransformGraph& transformParent);

        void setup(Renderer::RendererSystem& renderer, 
                    const Resources::Model* model, 
                    const Resources::Shader* shader, 
                    const Resources::Texture* texture,
                    Physics::TransformGraph& transformParent);

        void inputs(const Core::Engine& engine) override;

        // Returns true if the player dies
        void dealDamages(float damages);




        virtual void onCollisionEnter        (const SegmentHit& hit) override
        {
            std::cout << "player enter" << std::endl;
            state.playerState = PlayerState::E_IDLE;
        }

        virtual void onCollisionExit         (const SegmentHit& hit) override
        {
            std::cout << "player exit" << std::endl;
            state.playerState = PlayerState::E_JUMPING;
        }

        virtual void onOverlapEnterSelfHit   (const SegmentHit& hit) override
        {
            if (hit.normal.y < 0.5)
            {
                dealDamages(1.f);
            }
        }

        virtual void onOverlapEnterAnotherHit(const SegmentHit& hit) override
        {
            if (hit.normal.y > - 0.5)
            {
                dealDamages(1.f);
            }
        }
    };
}

#endif