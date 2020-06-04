#ifndef _FILE_HPP_
#define _FILE_HPP_

#include <string>

namespace Resources
{
    class File
    {
    public:
        // - filename : name of the file you  want to load
        // - outputText : returns the text contained in the file. 
        // Returns true if the corresponding file is loaded, false otherwise.
        static bool loadTextFromFile(const char* filename, std::string& outputText);

        // It does what you think it does.
        // Returns true if the corresponding file exists.
        static bool doesFileExist(const char* filename);
    };

}

#endif