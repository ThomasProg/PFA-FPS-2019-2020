#ifndef _DECORATION_HPP_
#define _DECORATION_HPP_

#include "collisionComponentInterface.hpp"
#include "renderableInterface.hpp"
#include "transformInterface.hpp"

#include  "demoResourceManager.hpp"

namespace Entity
{
    // Only renders objects.
    // Used for trees, rocks, etc
    class Decoration final : public Physics::TransformInterface,
                             public Renderer::RenderableInterface
    {
    private:
        static constexpr Core::Maths::Vec4 defaultColor = {0.1f, 0.450f, 0.1f,1};

    public:
        Decoration();
        Decoration(const Core::Maths::Vec4& color);

        void setResources(const DemoResourceManager&, E_Model model = E_Model::E_TREE, E_Shader shader = E_Shader::E_LIGHTED_FLATCOLOR);
    };
}

#endif