#include "phongFlat.hpp"

#include "mesh.hpp"
#include "lightManager.hpp"

#include "log.hpp"

Resources::PhongFlat::PhongFlat(const char* vsFilename, const char* fsFilename)
    : Shader(vsFilename, fsFilename)
{
    loadUniformValuesLocation();
}

void Resources::PhongFlat::loadUniformValuesLocation()
{
    nbCurrentLightsID = getUniformLocation("nbCurrentLights");
    if (nbCurrentLightsID == GLint(GL_INVALID_INDEX))
        Core::Debug::Log::addMessage("SetUniformValueFail");

    // ===== ===== ===== //

    mvpID = getUniformLocation("mvp");  

    modelID = getUniformLocation("model");  

    colorID = getUniformLocation("color");

    lightsBlockID = glGetUniformBlockIndex(getProgramID(), "lightsBlock");
    if (nbCurrentLightsID == GLint(GL_INVALID_INDEX))
        Core::Debug::Log::addMessage("SetUniformValueFailLightBlocks");
}

void Resources::PhongFlat::useUniformValues(const Renderer::Camera& cam, const Renderer::Mesh& mesh) const
{
    Core::Maths::Matrix4x4 mvp = (cam.viewProjectionMatrix * mesh.transform->transformMatrixNode->getWorldMatrix());
    mvp.transposeThis();

    glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

    glUniformMatrix4fv(modelID, 1, GL_FALSE, &mesh.transform->transformMatrixNode->getWorldMatrix().transpose()[0][0]);

    glUniform4fv(colorID, 1, &mesh.color.components[0]);
}

void Resources::PhongFlat::useLightsUniformValues(const Renderer::LightManager& lightManager) const 
{
    glUniform1i(nbCurrentLightsID, lightManager.getLights().size());

    for (uint i = 0; i < lightManager.getLights().size(); i++)
    {
        glUniformBlockBinding(getProgramID(), lightsBlockID, 0);

        glBindBuffer(GL_UNIFORM_BUFFER, lightManager.lightsUniformBuffer);
        glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(Renderer::LightData), sizeof(Renderer::LightData), &lightManager.getLights()[i]);
        glBindBufferBase(GL_UNIFORM_BUFFER, lightsBlockID, lightManager.lightsUniformBuffer);
    }
}