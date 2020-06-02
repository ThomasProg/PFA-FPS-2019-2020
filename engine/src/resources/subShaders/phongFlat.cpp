#include "phongFlat.hpp"

#include "mesh.hpp"
#include "lightManager.hpp"

void Resources::PhongFlat::loadUniformValuesLocation()
{
    nbCurrentLightsID = getUniformLocation("nbCurrentLights");
    if (nbCurrentLightsID == -1)
        std::cout << "Could not set uniform value : " "nbCUrrentLights\n";

    // ===== ===== ===== //

    mvpID = getUniformLocation("mvp");  

    modelID = getUniformLocation("model");  

    colorID = getUniformLocation("color");

    lightsBlockID = glGetUniformBlockIndex(programID, "lightsBlock");
}

void Resources::PhongFlat::useUniformValues(const Renderer::Camera& cam, const Renderer::Mesh& mesh) const
{
    Core::Maths::Matrix4x4 mvp = (cam.viewProjectionMatrix * mesh.transform->transformMatrixNode->worldData);
    mvp.transposeThis();
    
    // ===== Lights ===== //

    // glUniform1i(nbCurrentLightsID, lightManager.lights.size());

    // ===== ===== ===== //

    glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

    glUniformMatrix4fv(modelID, 1, GL_FALSE, &mesh.transform->transformMatrixNode->worldData.transpose()[0][0]);

    // TODO : change &mesh.color.x to &mesh.components (or something like that)
    glUniform4fv(colorID, 1, &mesh.color.x);
}

void Resources::PhongFlat::useLightsUniformValues(const Renderer::LightManager& lightManager) const 
{
    glUniform1i(nbCurrentLightsID, lightManager.lights.size());

    for (uint i = 0; i < lightManager.lights.size(); i++)
    {
        // if (lightsBlockID == GL_INVALID_INDEX)   
        //     std::cout << "\"lightsBlock\" variable doesn't exist or was removed!\n";
        //     // Core::Debug::Log::addMessage(_LOG_ERROR_("\"lightsBlock\" variable doesn't exist or was removed!"), true);
        // else 
        // {
            glUniformBlockBinding(programID, lightsBlockID, 0);

            glBindBuffer(GL_UNIFORM_BUFFER, lightManager.lightsUniformBuffer);
            glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(Renderer::LightData), sizeof(Renderer::LightData), &lightManager.lights[i]);
            glBindBufferBase(GL_UNIFORM_BUFFER, lightsBlockID, lightManager.lightsUniformBuffer);
        // }
    }
}