#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include "rendererSystem.hpp"

#include <unordered_map>

namespace Resources
{

    class Scene
    {
    public:
        virtual void load()     {}
        virtual void inputs()   {}
        virtual void update()   {}
        virtual void renderUI() = 0;

        // we can't render scenes in a good depth order if we have multiple scenes (without depth buffer) 
        virtual void render()   {}

        virtual ~Scene() = default;
    };

}

#endif