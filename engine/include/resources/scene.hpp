#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include "entity.hpp"

#include "rendererSystem.hpp"

#include <unordered_map>

namespace Resources
{

    class Scene
    {
    protected:
        std::unordered_map<unsigned int, Entity::Entity> entities;

        Renderer::RendererSystem rendererSystem;
        // CollisionSystem 

    public:
        virtual void inputs()   = 0;
        virtual void update()   = 0;
        virtual void renderUI() = 0;

        // TODO : remove render function
        // we can't render scenes in a good depth order if we have multiple scenes
        virtual void render()   = 0;
    };

}

#endif