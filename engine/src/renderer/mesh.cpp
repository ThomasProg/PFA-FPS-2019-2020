#include "mesh.hpp"

#include "shader.hpp"
#include "model.hpp"
#include "texture.hpp"

#include "matrix4x4.hpp"

#include "saver.hpp"
#include "loader.hpp"

#include "light.hpp"
#include "lightManager.hpp"

Renderer::Mesh::Mesh(Physics::GTransform* transform)
    : transform(transform)
{

}

void Renderer::Mesh::draw(const Renderer::Camera& cam, Renderer::LightManager& lightManager)
{
    if (transform == nullptr)
        return;
        
    if (!isDrawn || model == nullptr || shader == nullptr || 
        &(*transform->transformMatrixNode) == nullptr || !transform->transformMatrixNode.isValid())
        return;

    glBindVertexArray(model->VAO);


    if(texture != nullptr)
        glBindTexture(GL_TEXTURE_2D, texture->textureData);

    shader->useUniformValues(cam, *this);

    glDrawArrays(GL_TRIANGLES, 0, model->positions.size());
}

void Renderer::Mesh::linkShaderWithModel()
{
    assert(shader != nullptr && model != nullptr);
    shader->linkModel(*model);
}
