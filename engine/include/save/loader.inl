#include "loader.hpp"

// This function can be used to load a variable in the file easily.
// Pointers are not accepted, since they become invalid after loaded.
template<typename T, typename>
inline void Save::Loader::load(T& loadedData)
{
    loadedFile.read((char*) &loadedData, sizeof(T));
}

// Loads array defined by a ptr
template<typename T, typename>
inline void Save::Loader::loadPtr(const T loadedData, std::streamsize& nbElements)
{
    load(nbElements);
    loadedFile.read((char*) loadedData, nbElements * sizeof(*loadedData));
}

// Loads array defined by a ptr
template<typename T, typename>
inline void Save::Loader::loadPtr(const T loadedData)
{
    std::streamsize nbElements;
    load(nbElements);
    loadedFile.read((char*) loadedData, nbElements * sizeof(*loadedData));
}

inline void Save::Loader::tryToDisplayError(const char* errorMessageAdded) const 
{
    if(!loadedFile.good()) 
    {
        Core::Debug::Log::addMessage("An error occurred while loading data " + std::string(errorMessageAdded));
    }
}

