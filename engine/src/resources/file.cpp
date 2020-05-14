#include "file.hpp"

#include "log.hpp"

#include <fstream>
#include <sstream>

bool Resources::File::loadTextFromFile(const char* filename, std::string& outputText)
{
    std::ifstream vsStream(filename, std::ios::in);
    if (vsStream.is_open())
    {
        std::stringstream sstr;
        sstr << vsStream.rdbuf();
        outputText = sstr.str();
        vsStream.close();
        return true;
    }
    else 
    {
        Core::Debug::Log::addMessage(_LOG_ERROR_("couldn't load file : " + std::string(filename)));
        return false;
    }
}

bool Resources::File::doesFileExist(const char* filename)
{
    if (FILE* file = fopen(filename, "r"))
    {
        fclose(file);
        return true;
    }
    else 
        return false;
}