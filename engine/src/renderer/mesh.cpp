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
        
    if (!isDrawn || model == nullptr || shader == nullptr || &(*transform->transformMatrixNode) == nullptr || !transform->transformMatrixNode.isValid())
        return;

    // shader->use();
    glBindVertexArray(model->VAO);


    if(texture != nullptr)
        glBindTexture(GL_TEXTURE_2D, texture->textureData);

    Core::Maths::Matrix4x4 mvp = (cam.viewProjectionMatrix * transform->transformMatrixNode->worldData);
    mvp.transposeThis();
    
    // ===== Lights ===== //

    unsigned int i = 0;
    for (const Renderer::Light& light : lightManager.lights)
    {
        // light.lightData.location = Core::Maths::Vec4{cam.transform.transformMatrixNode->worldData.getTranslationVector(), 1};
        shader->linkLight(i, light.lightData, lightManager.lightsUniformBuffer);
        ++i;
    }

    GLint nbCurLights = shader->getUniformLocation("nbCurrentLights");
    if (nbCurLights != -1)
        glUniform1i(nbCurLights, lightManager.lights.size());
    else 
        std::cout << "Could not set uniform value : " "nbCUrrentLights" << std::endl;
        // Core::Debug::Log::addMessage(_LOG_ERROR_("Could not set uniform value : " "nbCurrentLights"));

    // ===== ===== ===== //

    GLuint mvpID = shader->getUniformLocation("mvp");  
    glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

    GLuint modelID = shader->getUniformLocation("model");  
    glUniformMatrix4fv(modelID, 1, GL_FALSE, &transform->transformMatrixNode->worldData.transpose()[0][0]);

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
