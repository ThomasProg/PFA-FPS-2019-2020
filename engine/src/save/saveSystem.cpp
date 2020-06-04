#include "saveSystem.hpp"

#include "saveInterface.hpp"

#include "loader.hpp"
#include "saver.hpp"
#include  "log.hpp"

#include <fstream>
#include <iostream>

Save::SaveSystem::iterator Save::SaveSystem::add(SaveInterface* savedItem)
{
    savedItems.push_back(savedItem);
    return savedItems.end();
}

void Save::SaveSystem::remove(const iterator& it)
{
    savedItems.erase(it);
}



void Save::SaveSystem::save(const char* filename)
{
    try 
    {
    Saver saver {std::ofstream{filename, std::ios::out | std::ios::binary}};
    if(!saver.saveFile) 
    {
        Core::Debug::Log::addMessage("Can't open file.");
        return;
    }

    for (SaveInterface* it : savedItems)
    {
        it->save(saver);
    }

    if(!saver.saveFile.good()) 
    {
        Core::Debug::Log::addMessage("An error occurred while saving items.");
        return;
    }
    }
    catch (...) { /* */ }
}

// loads data and then load links (see loadData() and loadLinks())
void Save::SaveSystem::load(const char* filename)
{
    loadData(filename);
}

void Save::SaveSystem::loadData(const char* filename)
{
    try 
    {
    Loader loader {std::ifstream{filename, std::ios::out | std::ios::binary}};
    if(!loader.loadedFile) 
    {
        Core::Debug::Log::addMessage("Can't open file.");
        return;
    }

    for (SaveInterface* it : savedItems)
    {
        it->loadData(loader);
    }

    if(!loader.loadedFile.good()) 
    {
        Core::Debug::Log::addMessage("An error occurred while loading data.");
        return;
    }
    }
    catch (...) { /* */ }
}

