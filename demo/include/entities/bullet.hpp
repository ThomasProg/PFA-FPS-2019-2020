#ifndef _BULLET_HPP_
#define _BULLET_HPP_

#include "demoResourceManager.hpp"

#include "transformInterface.hpp"
#include "renderableInterface.hpp"

namespace Entity
{
    class Bullet : public Physics::TransformInterface,
                   public Renderer::RenderableInterface
    {
    private:
        static constexpr Core::Maths::Vec4 defaultColor = {0.988f, 1.f, 0.478f, 1};

        static constexpr E_Model defaultModel = E_Model::E_BOX;
        static constexpr E_Shader defaultShader = E_Shader::E_LIGHTED_FLATCOLOR;

    public:
        static constexpr float lifeTime = 2.0f;

        float timer;
        
        Bullet();

        void setResources(const DemoResourceManager&);

        ~Bullet() = default;
    };
}

#endif