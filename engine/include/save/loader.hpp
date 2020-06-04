#ifndef _LOADER_HPP_
#define _LOADER_HPP_

#include <fstream>
#include <iostream>
namespace Save
{
    // Used to load data to binary easily.
    struct Loader
    {
        std::ifstream loadedFile;

        // This function can be used to load a variable in the file easily.
        // Pointers are not accepted, since they become invalid after loaded.
        template<typename T, typename = std::enable_if_t<!std::is_pointer<T>::value>>
        inline void load(T& loadedData);

        // Loads array defined by a ptr
        template<typename T, typename = std::enable_if_t<std::is_pointer<T>::value>>
        inline void loadPtr(const T loadedData, std::streamsize& nbElements);

        // Loads array defined by a ptr
        template<typename T, typename = std::enable_if_t<std::is_pointer<T>::value>>
        inline void loadPtr(const T loadedData);

        inline void tryToDisplayError(const char* errorMessageAdded) const;
    };
}

#endif