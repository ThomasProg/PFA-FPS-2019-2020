#ifndef _FILE_HPP_
#define _FILE_HPP_

#include <string>

namespace Resources
{

    class File
    {
    private:

    public:
        static bool loadTextFromFile(const char* filename, std::string& outputText);
        static bool doesFileExist(const char* filename);
    };

}

#endif