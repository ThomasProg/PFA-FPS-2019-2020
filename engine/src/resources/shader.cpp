#include "shader.hpp"

#include "lightData.hpp"

#include "file.hpp"
#include "log.hpp"

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace Resources;

Shader::Shader(const char* vsFilename, const char* fsFilename)
{
    // TODO : check if returning false
    GLuint vertexShader, fragmentShader;
    bool b1 = loadVertexShader(vsFilename, vertexShader);
    b1 &= loadFragmentShader(fsFilename, fragmentShader);

    if (!b1) 
        std::cout << "Error loading shaders" << std::endl;

    programID = createProgram(vertexShader, fragmentShader);
}

Shader::~Shader()
{
    if (!isValid())
        glDeleteProgram(programID);
}

Shader::Shader(Shader&& rhs)
{
    programID = rhs.programID;
    rhs.programID = 0;
}

Shader& Shader::operator=(Shader&& rhs)
{
    programID = rhs.programID;
    rhs.programID = 0;
    return *this;
}

bool Shader::loadShader(const std::string& filename, GLuint shaderType, GLuint& outID)
{
    Core::Debug::Log::addTitle("Loading Shader : " + filename);

    std::string s;
    if (Resources::File::loadTextFromFile(filename.c_str(), s))
    {
        outID = glCreateShader(shaderType);
        const char* source = s.c_str();

        glShaderSource(outID, 1, &source, NULL);
        glCompileShader(outID);

        Core::Debug::Log::printShaderError(outID);
        return true;
    }
    else
    {  
        Core::Debug::Log::addMessage(_LOG_ERROR_(
            "Couldn't load shader file : " + filename + " of shaderType : " + std::to_string(shaderType)), true);
        return false;
    }
}

bool Shader::loadVertexShader(const std::string& filename, GLuint& outID)
{
    return loadShader(filename, GL_VERTEX_SHADER, outID);
}

bool Shader::loadFragmentShader(const std::string& filename, GLuint& outID)
{
    return loadShader(filename, GL_FRAGMENT_SHADER, outID);
}
GLuint Shader::createProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint programID = glCreateProgram();

    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);

    glLinkProgram(programID);

    Core::Debug::Log::addTitle("Program");
    Core::Debug::Log::printProgramError(programID);

    glDetachShader(programID, vertexShader);
    glDeleteShader(vertexShader);

    glDetachShader(programID, fragmentShader);
    glDeleteShader(fragmentShader);

    return programID;
}


void Resources::Shader::setAttributePointer(GLuint programID, GLuint attributeVBOID, 
                                            const char* attributeName, unsigned int nbElemsPerStride) const
{
    GLint attributeID = glGetAttribLocation(programID, attributeName);
    if (attributeID == -1) 
    {
        Core::Debug::Log::addMessage(_LOG_ERROR_("Could not bind attribute : " + attributeName));
    }
    else 
    {
        glBindBuffer(GL_ARRAY_BUFFER, attributeVBOID);
        glEnableVertexAttribArray(attributeID);
        glVertexAttribPointer(
            attributeID,       // attribute
            nbElemsPerStride,  // number of elements per vertex, here (r,g,b)
            GL_FLOAT,          // the type of each element
            GL_FALSE,          // take our values as-is
            0,                 // no extra data between each position
            0                  // offset of first element
        );
    }
}

void Resources::Shader::linkModel(const Resources::Model& model) const 
{
    glBindVertexArray(model.VAO);

    setAttributePointer(programID, model.VBO, "v_loc", 3);

    // if (isTextured)
    // {
    //     // setAttributePointer(programID, model.colorID,    "v_color",   3);
    //     setAttributePointer(programID, model.uvsID,      "v_uvs",     2);
    //     setAttributePointer(programID, model.normalsID,  "v_normals", 3);
    // }

    glBindVertexArray(0);
}

// void Resources::Shader::linkTexture(const Texture<>&texture, const GLuint VAO) const
// {
//     glBindVertexArray(VAO);

//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, texture.textureID);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//     if (texture.nbComposites == 4) // in case of RGBA image
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.pixels);
//     else if (texture.nbComposites == 3) // in case of RGB image
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.pixels);
    
//     glGenerateMipmap(GL_TEXTURE_2D);

//     glBindVertexArray(0);
// }

void Resources::Shader::linkLight(unsigned int lightID, const Renderer::LightData& lightData, GLuint lightsUniformBuffer) const
{
    GLuint lightsID = glGetUniformBlockIndex(programID, "lightsBlock");
    if (lightsID == GL_INVALID_INDEX)   
        std::cout << "\"lightsBlock\" variable doesn't exist or was removed!" << std::endl;
        // Core::Debug::Log::addMessage(_LOG_ERROR_("\"lightsBlock\" variable doesn't exist or was removed!"), true);
    else 
    {
        glUniformBlockBinding(programID, lightsID, 0);

        glBindBuffer(GL_UNIFORM_BUFFER, lightsUniformBuffer);
        glBufferSubData(GL_UNIFORM_BUFFER, lightID * sizeof(Renderer::LightData), sizeof(Renderer::LightData), &lightData);
        glBindBufferBase(GL_UNIFORM_BUFFER, lightsID, lightsUniformBuffer);
    }
}