#ifndef _LOG_HPP_
#define _LOG_HPP_

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <string>

// Use this to get an error message.
// Example : 
//      Core::Debug::Log::addMessage(_LOG_ERROR_("This is an error."));
//
// To print the file, the line, and the function, this has to be a macro.
#define _LOG_ERROR_(message) std::string("\nERROR :\n\tFile : " __FILE__ "\n\tLine : " ) \
                            + std::to_string(__LINE__) \
                            + std::string("\n\tFunction : " + std::string(__FUNCTION__)) \
                            + "\n\tDescription : " + message + '\n'



namespace Core::Debug 
{
    class Log
    {
    private:
        static std::string logFile;
    public:
        // Sets new log file name with year/month/day added to it.
        // This must be called at the beginning.
        static void newLog(const std::string& logFilename = logFile) noexcept;
        // Prints shaders errors giving shader ID.
        static void printShaderError(GLuint id) noexcept;
        // Prints program errors giving shader ID.
        static void printProgramError(GLuint id) noexcept;
        // Prints str to log file with title nice look.
        static void addTitle(const std::string& str) noexcept;
        // Prints str to log file and prints it in terminal if printInTerminal is true.
        static void addMessage(const std::string& str, bool printInTerminal = false) noexcept; 
        // Prints glGetError() messages to log file 
        static void printGLError() noexcept;
    };
}

#endif