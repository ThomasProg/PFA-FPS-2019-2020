#include "shader.hpp"
#include <iostream>

inline GLuint Resources::Shader::getProgramID() const noexcept
{
    return programID;
}

inline constexpr bool Resources::Shader::isValid() const noexcept
{
    return programID != 0;
}

inline GLint Resources::Shader::getUniformLocation(const char* str) const
{
    return glGetUniformLocation(programID, str);
}

inline void Resources::Shader::use() const
{
    glUseProgram(programID);
}