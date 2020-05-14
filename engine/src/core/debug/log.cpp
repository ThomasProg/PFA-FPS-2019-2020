#include "log.hpp"

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <ctime>
#include <vector>

#include <fstream>

std::string Core::Debug::Log::logFile = "";

void Core::Debug::Log::printShaderError(GLuint id) noexcept
{
    GLint result = GL_FALSE;
    int logLength = 0;

    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength > 0)
    {
        std::vector<GLchar> errorLog(logLength + 1);
        glGetShaderInfoLog(id, logLength, NULL, &errorLog[0]);
        Core::Debug::Log::addMessage(_LOG_ERROR_("Error : " + std::string(&errorLog[0])));
    }
}

void Core::Debug::Log::printProgramError(GLuint id) noexcept
{
    GLint isLinked;

    glGetProgramiv(id, GL_LINK_STATUS, &isLinked);

    if (isLinked == GL_FALSE)
    {
        int logLength = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> errorLog (logLength + 1);
        glGetProgramInfoLog(id, logLength, NULL, &errorLog[0]);
        Core::Debug::Log::addMessage(_LOG_ERROR_("Error : " + std::string(&errorLog[0])));
    }
}

void Core::Debug::Log::newLog(const std::string& logFilename) noexcept
{
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    logFile = logFilename + '_' + 
                std::to_string(now->tm_year + 1900) + '_' + 
                std::to_string(now->tm_mon + 1) + '_' + 
                std::to_string(now->tm_mday) + ".txt";
    std::remove(logFilename.c_str());
}

void Core::Debug::Log::addTitle(const std::string& str) noexcept
{
    std::ofstream logFile;
    logFile.open(Core::Debug::Log::logFile.c_str(), std::ios_base::app);

    const char borders[] = "=========";

    for (unsigned int i = 0; i < str.size() - 2 + 2 * sizeof(borders); ++i)
        logFile << '=';
    logFile << '\n';

    logFile << borders << str << borders << '\n';

    for (unsigned int i = 0; i < str.size() - 2 + 2 * sizeof(borders); ++i)
        logFile << '=';
    logFile << "\n\n";

    logFile.close();
}
#include <iostream>
void Core::Debug::Log::addMessage(const std::string& str, bool printInTerminal) noexcept
{
    std::ofstream logFile;
    logFile.open(Core::Debug::Log::logFile.c_str(), std::ios_base::app);

    logFile << str << '\n';

    #if _ENGINE_DEBUG_
    if (printInTerminal)
        std::cout << str << '\n';
    #endif
    std::cout << str << std::endl;

    logFile.close();
}

void Core::Debug::Log::printGLError() noexcept
{
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::string errorName;
        switch (err)
        {
            case 1280 : 
                errorName = "Enum parameter is not legal";
                break;
            case 1281 : 
                errorName = "Value parameter is not legal";
                break;
            case 1282 : 
                errorName = "State for command not legal for given parameters";
                break;
            case 1283 : 
                errorName = "Stack pushing operation is causing a stack overflow";
                break;
            case 1284 : 
                errorName = "Stack popping operation occus while the stack is at its lowest point";
                break;
            case 1285 : 
                errorName = "Memory allocation operation cannot allocate (enough) memory";
                break;
            case 1286 : 
                errorName = "Reading or writing to a framebuffer that is not complete";
                break;
        }
        Core::Debug::Log::addMessage(_LOG_ERROR_("OpenGL error : " + std::to_string(err) + " / " + errorName), true);
        std::cout << "error : " << err << std::endl;
    }
}
