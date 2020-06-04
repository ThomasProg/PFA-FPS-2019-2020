#include "saver.hpp"

#include "log.hpp"

// This function can be used to write a variable in the file easily.
// Pointers are not accepted, since they become invalid after loaded.
template<typename T, typename>
inline void Save::Saver::save(const T& savedData)
{
    saveFile.write((char*) &savedData, sizeof(T));
}

// Loads array defined by a ptr
template<typename T, typename>
inline void Save::Saver::savePtr(const T savedData, const std::streamsize nbElements)
{
    save(nbElements);
    saveFile.write((char*) savedData, nbElements * sizeof(*savedData));
}

inline void Save::Saver::tryToDisplayError(const char* errorMessageAdded) const 
{
    if(!saveFile.good()) 
    {
        Core::Debug::Log::addMessage("An error occurred while saving data " + std::string(errorMessageAdded));
        return;
    }
}

