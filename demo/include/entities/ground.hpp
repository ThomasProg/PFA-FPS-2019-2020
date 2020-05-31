#ifndef _GROUND_HPP_
#define _GROUND_HPP_

#include "staticObject.hpp"

#include  "demoResourceManager.hpp"

namespace Entity
{
    class Ground final : public StaticObject
    {
    public:
        Ground() 
            : StaticObject()
        {
            mesh.color = Core::Maths::Vec4{0.835f, 0.650f, 0.384f,1};
        }

        Ground(const Core::Maths::Vec4& color) 
            : StaticObject()
        {
            mesh.color = color;
        }

        void setResources(const DemoResourceManager&);
    };
}

#endif