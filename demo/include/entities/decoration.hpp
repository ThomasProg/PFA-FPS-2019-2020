#ifndef _DECORATION_HPP_
#define _DECORATION_HPP_

#include "staticObject.hpp"

#include  "demoResourceManager.hpp"

namespace Entity
{
    class Decoration final : public StaticObject
    {
    public:
        Decoration() 
            : StaticObject()
        {
            mesh.color = Core::Maths::Vec4{0.1f, 0.450f, 0.1f,1};
        }

        Decoration(const Core::Maths::Vec4& color) 
            : StaticObject()
        {
            mesh.color = color;
        }

        void setResources(const DemoResourceManager&, E_Model model = E_Model::E_TREE, E_Shader shader = E_Shader::E_FLAT);
    };
}

#endif