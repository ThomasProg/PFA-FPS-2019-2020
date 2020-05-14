#include "shader.hpp"
#include <iostream>
inline constexpr bool Resources::Shader::isValid() const noexcept
{
    return programID != 0;
}

inline GLuint Resources::Shader::getUniformLocation(const char* str) const
{
    return glGetUniformLocation(programID, str);
}

inline void Resources::Shader::use() const
{
    glUseProgram(programID);
}