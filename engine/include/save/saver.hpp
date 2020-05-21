#ifndef _SAVER_HPP_
#define _SAVER_HPP_

#include <fstream>
#include <iostream>

namespace Save
{
    struct Saver
    {
        std::ofstream saveFile;

        // This function can be used to write a variable in the file easily.
        // Pointers are not accepted, since they become invalid after loaded.
        template<typename T, typename = std::enable_if_t<!std::is_pointer<T>::value>>
        void save(const T& savedData)
        {
            saveFile.write((char*) &savedData, sizeof(T));
        }

        // Loads array defined by a ptr
        template<typename T, typename = std::enable_if_t<std::is_pointer<T>::value>>
        void savePtr(const T savedData, const std::streamsize nbElements)
        {
            save(nbElements);
            saveFile.write((char*) savedData, nbElements * sizeof(*savedData));
        }

        inline void tryToDisplayError(const char* errorMessageAdded) const 
        {
            if(!saveFile.good()) 
            {
                std::cout << "An error occurred while saving data " << errorMessageAdded << std::endl;
                return;
            }
        }
    };
}

#endif