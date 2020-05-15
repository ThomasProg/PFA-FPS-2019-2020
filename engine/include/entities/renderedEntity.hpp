#ifndef _RENDERED_ENTITY_HPP_
#define _RENDERED_ENTITY_HPP_

#include "entity.hpp"
#include "rendererSystem.hpp"

namespace Entity
{
    class renderedEntity: public Entity
    {
    private:
        Physics::GraphKey key;
        Renderer::MeshIt mesh;
    public:
        renderedEntity();
        ~renderedEntity();
    };
}

#endif