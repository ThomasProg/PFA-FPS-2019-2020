#include "renderedEntity.hpp"


void Entity::RenderedEntity::setup(Renderer::RendererSystem& renderer, 
            const Resources::Model* model, 
            const Resources::Shader* shader, 
            Physics::TransformGraph& transformParent) 
{
    if (!mesh.isValid())
    {
        mesh = renderer.addComponentTo(*this);
    }

    // transform 
    mesh->transform.transformMatrixNode = transformParent.addChild();

    // resources
    mesh->model  = model;
    mesh->shader = shader;
    mesh->linkShaderWithModel();
}

void Entity::RenderedEntity::setup(Renderer::RendererSystem& renderer, 
            const Resources::Model* model, 
            const Resources::Shader* shader,
            const Resources::Texture* texture,
            Physics::TransformGraph& transformParent) 
{
    setup(renderer, model, shader, transformParent);
    mesh->texture = texture;
}