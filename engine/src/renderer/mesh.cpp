#include "mesh.hpp"

#include "shader.hpp"
#include "model.hpp"
#include "texture.hpp"

#include "matrix4x4.hpp"

#include "saver.hpp"
#include "loader.hpp"

void Renderer::Mesh::draw(const Renderer::Camera& cam)
{
    if (!isDrawn || model == nullptr || shader == nullptr || &(*transform->transformMatrixNode) == nullptr)
        return;

    // Call Callback if valid
    if (onDraw)
        onDraw();

    glBindVertexArray(model->VAO);

    shader->use();

    if(texture != nullptr)
        glBindTexture(GL_TEXTURE_2D, texture->textureData);

    Core::Maths::Matrix4x4 mvp = (cam.projection * cam.transform.transformMatrixNode->worldData.getInverse() * transform->transformMatrixNode->worldData);
    mvp.transposeThis();
    
    GLuint mvpID = shader->getUniformLocation("mvp");  
    glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

    GLuint modelID = shader->getUniformLocation("model");  
    glUniformMatrix4fv(modelID, 1, GL_FALSE, &transform->transformMatrixNode->worldData[0][0]);

    GLuint colorID = shader->getUniformLocation("color");

    glUniform4fv(colorID, 1, &color[0]);

    // TODO : Put a variable member to know what mode we have to use
    // glDrawElements(GL_TRIANGLES, model->indices.size(), GL_UNSIGNED_INT, &model->indices[0]);
    glDrawArrays(GL_TRIANGLES, 0, model->positions.size());
}

void Renderer::Mesh::linkShaderWithModel()
{
    assert(shader != nullptr && model != nullptr);
    shader->linkModel(*model);
}
