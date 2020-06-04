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
    glBindVertexArray(model.vao);

    setAttributePointer(programID, model.vbo, "v_loc", 3);

    glBindVertexArray(0);
}
