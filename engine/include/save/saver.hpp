#ifndef _SAVER_HPP_
#define _SAVER_HPP_

#include <fstream>
#include <iostream>

namespace Save
{
    // Used to save data to binary easily.
    struct Saver
    {
        std::ofstream saveFile;

        // This function can be used to write a variable in the file easily.
        // Pointers are not accepted, since they become invalid after loaded.
        template<typename T, typename = std::enable_if_t<!std::is_pointer<T>::value>>
        inline void save(const T& savedData);

        // Loads array defined by a ptr
        template<typename T, typename = std::enable_if_t<std::is_pointer<T>::value>>
        inline void savePtr(const T savedData, const std::streamsize nbElements);

        // Displays errors if the savefile is not valid.
        inline void tryToDisplayError(const char* errorMessageAdded) const;
    };
}

#include "saver.inl"

#endif