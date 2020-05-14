#ifndef _LOADER_HPP_
#define _LOADER_HPP_

#include <fstream>
#include <iostream>
namespace Save
{
    struct Loader
    {
        std::ifstream loadedFile;

        // This function can be used to load a variable in the file easily.
        // Pointers are not accepted, since they become invalid after loaded.
        template<typename T, typename = std::enable_if_t<!std::is_pointer<T>::value>>
        void load(T& loadedData)
        {
            loadedFile.read((char*) &loadedData, sizeof(T));
        }

        // Loads array defined by a ptr
        template<typename T, typename = std::enable_if_t<std::is_pointer<T>::value>>
        void loadPtr(const T loadedData, std::streamsize& nbElements)
        {
            load(nbElements);
            loadedFile.read((char*) loadedData, nbElements * sizeof(*loadedData));
        }

        // Loads array defined by a ptr
        template<typename T, typename = std::enable_if_t<std::is_pointer<T>::value>>
        void loadPtr(const T loadedData)
        {
            std::streamsize nbElements;
            load(nbElements);
            loadedFile.read((char*) loadedData, nbElements * sizeof(*loadedData));
        }

        inline void tryToDisplayError(const char* errorMessageAdded) const 
        {
            if(!loadedFile.good()) 
            {
                std::cout << "An error occurred while loading data " << errorMessageAdded << std::endl;
                return;
            }
        }
    };
}

#endif